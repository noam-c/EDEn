/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Pathfinder.h"
#include "EntityGrid.h"
#include "Point2D.h"
#include "TileState.h"
#include <algorithm>
#include <cstdlib>
#include <chrono>
#include <tuple>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_PATHFINDER

const float Pathfinder::ROOT_2 = 1.41421356f;

Pathfinder::Pathfinder() :
   m_collisionGrid(nullptr),
   m_collisionGridBounds(nullptr)
{
}

void Pathfinder::initialize(const Grid<TileState>& grid, int tileSize, const geometry::Rectangle& gridBounds)
{
   DEBUG("Resetting pathfinder...");
   m_movementTileSize = tileSize;
   m_collisionGrid = &grid;
   m_collisionGridBounds = &gridBounds;
   
   m_royFloydWarshallCalculation.cancel();

   m_royFloydWarshallCalculation.runTask(
                                      &RoyFloydWarshallMatrices::calculateRoyFloydWarshallMatrices,
                                      m_collisionGrid,
                                      m_collisionGridBounds);
   DEBUG("Pathfinder reinitialized.");
}

bool Pathfinder::isRoyFloydWarshallCalculationReady() const
{
   if(!m_royFloydWarshallCalculation.valid())
   {
      return false;
   }

   const auto futureStatus = m_royFloydWarshallCalculation.waitFor(std::chrono::seconds(0));

   return futureStatus == std::future_status::ready;
}

Pathfinder::Path Pathfinder::findBestPath(const EntityGrid& entityGrid, const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size) const
{
   return isRoyFloydWarshallCalculationReady() ?
      findRFWPath(src, dst, m_royFloydWarshallCalculation.get()) :
      findAStarPath(entityGrid, src, dst, size);
}

Pathfinder::Path Pathfinder::findReroutedPath(const EntityGrid& entityGrid, const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size) const
{
   return findAStarPath(entityGrid, src, dst, size);
}

std::vector<geometry::Point2D> Pathfinder::getAdjacentPoints(const geometry::Point2D& point, const geometry::Rectangle& bounds)
{
   std::vector<geometry::Point2D> points;

   if(point.x > bounds.left)
   {
      // Left point
      points.emplace_back(point.x - 1, point.y);
   }

   if(point.x < bounds.right - 1)
   {
      // Right point
      points.emplace_back(point.x + 1, point.y);
   }

   if(point.y > bounds.top)
   {
      // Upper point
      points.emplace_back(point.x, point.y - 1);
   }

   if(point.y < bounds.bottom - 1)
   {
      // Lower point
      points.emplace_back(point.x, point.y + 1);
   }

   if(point.x > bounds.left && point.y > bounds.top)
   {
      // Upper-left point
      points.emplace_back(point.x - 1, point.y - 1);
   }

   if(point.x > bounds.left && point.y < bounds.bottom - 1)
   {
      // Lower-left point
      points.emplace_back(point.x - 1, point.y + 1);
   }

   if(point.x < bounds.right - 1 && point.y > bounds.top)
   {
      // Upper-right point
      points.emplace_back(point.x + 1, point.y - 1);
   }

   if(point.x < bounds.right - 1 && point.y < bounds.bottom - 1)
   {
      // Lower-right point
      points.emplace_back(point.x + 1, point.y + 1);
   }

   return points;
}

/**
 * Represents a node in an A* search. Tracks the predecessor node and expected cost
 * of the node to enable the A* algorithm to track the lowest cost node.
 *
 * @author Noam Chitayat
 */
class Pathfinder::AStarNode
{
private:

   /** The actual distance cost from the origin node to this node. */
   float gCost;

   /** The heuristic distance cost from this node to the destination node. */
   float hCost;

   /** The total expected cost of traveling through this node from the origin to the destination. */
   float fCost;

public:
   /** The point that this node represents. */
   geometry::Point2D point;

   /** The predecessor of this node (the previous node on the shortest discovered path to this node). */
   std::shared_ptr<AStarNode const> parent;

   /**
    * Constructor.
    *
    * @param point The tile coordinates of this node.
    * @param gCost The actual distance cost required to travel from the origin to this node.
    * @param hCost The estimated distance cost required to travel from this node to the destination.
    * @param parent The predecessor of this node on a path from the origin.
    */
   AStarNode(geometry::Point2D point, float gCost, float hCost, const std::shared_ptr<AStarNode> parent = std::shared_ptr<AStarNode>()) : gCost(gCost), hCost(hCost), fCost(gCost + hCost), point(point), parent(parent) {}

   /**
    * Sets a new actual travel cost from the origin to this node.
    *
    * @param newCost The new travel cost from the origin to this node.
    */
   void setGCost(float newCost)
   {
      gCost = newCost;
      fCost = gCost + hCost;
   }

   /**
    * Sets a new actual heuristic cost from this node to the destination.
    *
    * @param newCost The new heuristic cost from this node to the destination.
    */
   void setHCost(float newCost)
   {
      hCost = newCost;
      fCost = gCost + hCost;
   }

   /**
    * @return The actual distance cost required to travel from the origin to this node.
    */
   float getGCost() const
   {
      return gCost;
   }

   /**
    * @return The estimated distance cost required to travel from this node to the destination.
    */
   float getHCost() const
   {
      return hCost;
   }

   /**
    * @return The total expected distance cost required to use this node to travel between the origin and destination.
    */
   float getFCost() const
   {
      return fCost;
   }

   /**
    * Comparison operation between two A* nodes. The lowest cost node is the one
    * with the lowest total estimated cost (F cost). In the case of a tie, the
    * highest G cost wins since it is not as deep in the search tree.
    *
    * @param lhs An A* node to compare.
    * @param rhs Another A* node to compare.
    *
    * @return true iff lhs has a lower expected cost than rhs.
    */
   static bool isLowerPriority(const std::shared_ptr<AStarNode>& lhs, const std::shared_ptr<AStarNode>& rhs)
   {
      // We consider lhs to have a lower priority if it has a higher total f() cost.
      // In case of a tie, this point will have lower priority if it has a lower g() cost,
      // indicating that it is not as deep in the search tree.
      return lhs->fCost > rhs->fCost || (lhs->fCost == rhs->fCost && lhs->gCost < rhs->gCost);
   }
};

Pathfinder::Path Pathfinder::findAStarPath(const EntityGrid& entityGrid, const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size) const
{
   if(!m_collisionGrid || !m_collisionGridBounds || m_collisionGrid->empty()) return Path();

   const TileState& entityState = (*m_collisionGrid)(src.x / m_movementTileSize, src.y / m_movementTileSize);

   if(!entityGrid.canOccupyArea(geometry::Rectangle(dst, size), entityState)) return Path();

   const geometry::Point2D sourceTile = src / m_movementTileSize;
   const geometry::Point2D destinationTile = dst / m_movementTileSize;

   std::vector<std::shared_ptr<AStarNode>> openSet;

   Grid<int> discovered(m_collisionGridBounds->getSize(), 0);

   openSet.push_back(std::make_shared<AStarNode>(sourceTile, 0, 0));
   std::push_heap(openSet.begin(), openSet.end(), AStarNode::isLowerPriority);

   auto rfwMatrices =
      isRoyFloydWarshallCalculationReady() ?
      &m_royFloydWarshallCalculation.get() : nullptr;

   discovered(sourceTile.x, sourceTile.y) = 1;

   Path path;

   while(!openSet.empty())
   {
      // Get the lowest-cost point in the open set, and remove it from the open set
      std::shared_ptr<AStarNode> cheapestNode = openSet.front();
      std::pop_heap(openSet.begin(), openSet.end(), AStarNode::isLowerPriority);
      openSet.pop_back();

      if(cheapestNode->point == destinationTile)
      {
         DEBUG("Found goal point %d,%d", cheapestNode->point.x, cheapestNode->point.y);
         std::shared_ptr<AStarNode const> curr = cheapestNode;
         while(curr)
         {
            path.push_front(curr->point * m_movementTileSize);
            curr = curr->parent;
         }
         break;
      }

      DEBUG("Evaluating point %d,%d", cheapestNode->point.x, cheapestNode->point.y);

      evaluateAdjacentNodes(entityState, size, cheapestNode, destinationTile, entityGrid, rfwMatrices, openSet, discovered);
   }

   return path;
}

unsigned int Pathfinder::getManhattanDistance(const geometry::Point2D& src, const geometry::Point2D& dst)
{
   unsigned int xDistance = abs(dst.x - src.x);
   unsigned int yDistance = abs(dst.y - src.y);

   return xDistance + yDistance;
}

void Pathfinder::evaluateAdjacentNodes(const TileState& entityState, const geometry::Size& entitySize, const std::shared_ptr<AStarNode>& evaluatedNode, const geometry::Point2D& destinationTile, const EntityGrid& entityGrid, const RoyFloydWarshallMatrices *const rfwMatrices, std::vector<std::shared_ptr<AStarNode>>& openSet, Grid<int>& discovered) const
{
   const std::vector<geometry::Point2D> adjacentPoints = Pathfinder::getAdjacentPoints(evaluatedNode->point, *m_collisionGridBounds);
   for(auto& adjacentPoint : adjacentPoints)
   {
      bool diagonalMovement =
         adjacentPoint.x != evaluatedNode->point.x &&
         adjacentPoint.y != evaluatedNode->point.y;

      float tileGCost = evaluatedNode->getGCost() + (diagonalMovement ? ROOT_2 : 1.0f);
      float tileHCost = rfwMatrices ? rfwMatrices->getDistance(adjacentPoint, destinationTile) : Pathfinder::getManhattanDistance(adjacentPoint, destinationTile);
      if(discovered(adjacentPoint.x, adjacentPoint.y) == 0)
      {
         discovered(adjacentPoint.x, adjacentPoint.y) = 1;

         bool freeTile = entityGrid.canOccupyArea(geometry::Rectangle(adjacentPoint * m_movementTileSize, entitySize), entityState);

         if(diagonalMovement)
         {
            const geometry::Point2D horizontalDestinationPoint(evaluatedNode->point.x, adjacentPoint.y);
            const geometry::Point2D verticalDestinationPoint(adjacentPoint.x, evaluatedNode->point.y);

            freeTile = freeTile &&
               entityGrid.canOccupyArea(geometry::Rectangle(horizontalDestinationPoint * m_movementTileSize, entitySize), entityState) &&
               entityGrid.canOccupyArea(geometry::Rectangle(verticalDestinationPoint * m_movementTileSize, entitySize), entityState);
         }

         if(freeTile)
         {
            DEBUG("Pushing point %d,%d onto open set with parent point %d,%d and g()=%f and f()=%f.", adjacentPoint.x, adjacentPoint.y, evaluatedNode->point.x, evaluatedNode->point.y, tileGCost, tileGCost + tileHCost);
            openSet.push_back(std::make_shared<AStarNode>(adjacentPoint, tileGCost, tileHCost, evaluatedNode));
            std::push_heap(openSet.begin(), openSet.end(), AStarNode::isLowerPriority);
         }
      }
      else
      {
         const auto& openSetIter = std::find_if(
                                      openSet.begin(),
                                      openSet.end(),
                                      [&](const std::shared_ptr<AStarNode>& node)
                                      {
                                         return node->point == adjacentPoint;
                                      });

         if(openSetIter != openSet.end())
         {
            const auto& tileInOpenSet = *openSetIter;
            if(tileInOpenSet->getGCost() > tileGCost)
            {
               DEBUG("Altering cost of discovered point %d, %d to g()=%f and f()=%f", adjacentPoint.x, adjacentPoint.y, tileGCost, tileGCost + tileHCost);
               tileInOpenSet->setGCost(tileGCost);
               tileInOpenSet->parent = evaluatedNode;
               std::make_heap(openSet.begin(), openSet.end(), AStarNode::isLowerPriority);
            }
         }
      }
   }
}

Pathfinder::Path Pathfinder::findRFWPath(const geometry::Point2D& src, const geometry::Point2D& dst, const RoyFloydWarshallMatrices& rfwMatrices) const
{
   geometry::Point2D sourceTile = src / m_movementTileSize;
   geometry::Point2D destinationTile = dst / m_movementTileSize;

   Path path;

   for(;;)
   {
      auto nextTile = rfwMatrices.getSuccessor(sourceTile, destinationTile);
      if(!std::get<0>(nextTile))
      {
         break;
      }

      DEBUG("Pushing back tile %d,%d onto path", sourceTile.x, sourceTile.y);
      path.push_back(sourceTile * m_movementTileSize);
      sourceTile = std::get<1>(nextTile);
   }

   return path;
}
