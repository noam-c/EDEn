/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Pathfinder.h"
#include "EntityGrid.h"
#include "Point2D.h"
#include "TileState.h"
#include <limits>
#include <algorithm>

#include "DebugUtils.h"
const int debugFlag = DEBUG_PATHFINDER;

const float Pathfinder::ROOT_2 = 1.41421356f;
const float Pathfinder::INFINITY = std::numeric_limits<float>::infinity();

shapes::Point2D Pathfinder::tileNumToCoords(int tileNum) const
{
   div_t result = div(tileNum, m_collisionGridBounds.getWidth());
   return shapes::Point2D(result.rem, result.quot);
}

shapes::Point2D Pathfinder::tileNumToPixels(int tileNum) const
{
   shapes::Point2D p = tileNumToCoords(tileNum);
   return p * m_movementTileSize;
}

int Pathfinder::coordsToTileNum(const shapes::Point2D& tileLocation) const
{
   return (tileLocation.y * m_collisionGridBounds.getWidth() + tileLocation.x);
}

int Pathfinder::pixelsToTileNum(const shapes::Point2D& pixelLocation) const
{
   return coordsToTileNum(pixelLocation / m_movementTileSize);
}

Pathfinder::Pathfinder() :
   m_distanceMatrix(NULL),
   m_successorMatrix(NULL),
   m_collisionGrid(NULL)
{
}

void Pathfinder::initialize(TileState** grid, int tileSize, const shapes::Rectangle& gridBounds)
{
   DEBUG("Resetting pathfinder...");
   deleteRoyFloydWarshallMatrices();
   m_movementTileSize = tileSize;
   m_collisionGrid = grid;
   m_collisionGridBounds = gridBounds;
   initRoyFloydWarshallMatrices();
   DEBUG("Pathfinder reinitialized.");
}

Pathfinder::~Pathfinder()
{
   deleteRoyFloydWarshallMatrices();
}

void Pathfinder::initRoyFloydWarshallMatrices()
{
   const unsigned int NUM_TILES = m_collisionGridBounds.getArea();
   
   m_distanceMatrix = new float*[NUM_TILES];
   m_successorMatrix = new int*[NUM_TILES];

   for(unsigned int i = 0; i < NUM_TILES; ++i)
   {
      m_distanceMatrix[i] = new float[NUM_TILES];
      m_successorMatrix[i] = new int[NUM_TILES];
   }

   shapes::Point2D aTile;
   shapes::Point2D bTile;

   for(unsigned int a = 0; a < NUM_TILES; ++a)
   {
      aTile = tileNumToCoords(a);
      for(unsigned int b = 0; b < NUM_TILES; ++b)
      {
         if(a == b)
         {
            m_distanceMatrix[a][b] = 0;
            m_successorMatrix[a][b] = -1;
         }
         else
         {
            bTile = tileNumToCoords(b);

            bool xAdjacent = aTile.x - 1 <= bTile.x && bTile.x <= aTile.x + 1;
            bool yAdjacent = aTile.y - 1 <= bTile.y && bTile.y <= aTile.y + 1;
            
            bool adjacent = xAdjacent && yAdjacent;
            bool diagonallyAdjacent = aTile.x != bTile.x && aTile.y != bTile.y;
            bool aTileIsObstacle = m_collisionGrid[aTile.y][aTile.x].entityType == TileState::OBSTACLE;
            bool bTileIsObstacle = m_collisionGrid[bTile.y][bTile.x].entityType == TileState::OBSTACLE;

            m_distanceMatrix[a][b] = INFINITY;
            m_successorMatrix[a][b] = -1;

            if(adjacent && !aTileIsObstacle && !bTileIsObstacle)
            {
               if(diagonallyAdjacent)
               {
                  bool diagonalTraversalBlocked =
                        m_collisionGrid[bTile.y][aTile.x].entityType == TileState::OBSTACLE ||
                        m_collisionGrid[aTile.y][bTile.x].entityType == TileState::OBSTACLE;

                  if(!diagonalTraversalBlocked)
                  {
                     m_successorMatrix[a][b] = b;
                     m_distanceMatrix[a][b] = ROOT_2;
                  }
               }
               else
               {
                  m_successorMatrix[a][b] = b;
                  m_distanceMatrix[a][b] = 1;
               }
            }
         }
      }
   }

   for(unsigned int i = 0; i < NUM_TILES; ++i)
   {
      for(unsigned int a = 0; a < NUM_TILES; ++a)
      {
         for(unsigned int b = 0; b < NUM_TILES; ++b)
         {
            float distance = m_distanceMatrix[a][i] + m_distanceMatrix[i][b];
            if(distance < m_distanceMatrix[a][b])
            {
               m_distanceMatrix[a][b] = distance;
               m_successorMatrix[a][b] = m_successorMatrix[a][i];
            }
         }
      }
   }
}

Pathfinder::Path Pathfinder::findBestPath(const shapes::Point2D& src, const shapes::Point2D& dst) const
{
   return findRFWPath(src, dst);
}

Pathfinder::Path Pathfinder::findReroutedPath(const EntityGrid& entityGrid, const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size) const
{
   return findAStarPath(entityGrid, src, dst, size);
}

/**
 * Represents a node in an A* search. Tracks the predecessor node and expected cost
 * of the node to enable the A* algorithm to track the lowest cost node.
 *
 * @author Noam Chitayat
 */
class Pathfinder::AStarPoint : public shapes::Point2D
{
   private:
      /** The predecessor of this node (the previous node on the shortest discovered path to this node). */
      const AStarPoint* parent;
      
      /** The actual distance cost from the origin node to this node. */
      float gCost;
   
      /** The heuristic distance cost from this node to the destination node. */
      float hCost;

      /** The total expected cost of traveling through this node from the origin to the destination. */
      float fCost;

   public:
      /**
       * Constructor.
       *
       * @param parent The predecessor of this node on a path from the origin.
       * @param x The x coordinate of this node.
       * @param y The y coordinate of this node.
       * @param gCost The actual distance cost required to travel from the origin to this node.
       * @param hCost The estimated distance cost required to travel from this node to the destination.
       */
      AStarPoint(const AStarPoint* parent, int x, int y, float gCost, float hCost) : shapes::Point2D(x,y), parent(parent), gCost(gCost), hCost(hCost), fCost(gCost + hCost) {}

      /**
       * Sets a new predecessor node for this node (if a shorter path to this node is found.)
       *
       * @param newParent The new parent node used to travel to this node.
       */
      void setParent(const AStarPoint* newParent)
      {
         parent = newParent;
      }

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
       * Retrieves the previous node on the shortest currently discovered path from
       * the origin to the destination.
       *
       * @return the predecessor of this node on the shortest discovered path.
       */
      const AStarPoint* getParent() const
      {
         return parent;
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
       * A comparator used to sort A* nodes in a priority queue.
       * Given two A* nodes, determines which A* node is currently the lowest cost to use.
       *
       * @author Noam Chitayat
       */
      struct IsLowerPriority
      {
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
         bool operator()(const AStarPoint* lhs, const AStarPoint* rhs) const
         {
            // We consider lhs to have a lower priority if it has a higher total f() cost.
            // In case of a tie, this point will have lower priority if it has a lower g() cost,
            // indicating that it is not as deep in the search tree.
            return lhs->fCost > rhs->fCost || (lhs->fCost == rhs->fCost && lhs->gCost < rhs->gCost);
         }
      };

      /**
       * Equality operation.
       * Used to determine equality between a coordinate pair and an A* node representing a coordinate.
       *
       * @author Noam Chitayat
       */
      struct ArePointsEqual
      {
         /** The coordinates to compare to. */
         const shapes::Point2D& point;
         
         /**
          * @return true iff this A* node represents the same location as point.
          */
         bool operator()(const AStarPoint* other) const
         {
            return point == *other;
         }
      };
};

Pathfinder::Path Pathfinder::findAStarPath(const EntityGrid& entityGrid, const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size) const
{
   if(m_collisionGrid == NULL) return Path();

   const TileState& entityState = m_collisionGrid[src.y / m_movementTileSize][src.x / m_movementTileSize];

   if(!entityGrid.canOccupyArea(shapes::Rectangle(dst, size), entityState)) return Path();

   shapes::Point2D destinationPoint(dst.x / m_movementTileSize, dst.y / m_movementTileSize);

   std::vector<AStarPoint*> openSet;
   std::vector<const AStarPoint*> closedSet;
   
   const int NUM_TILES = m_collisionGridBounds.getArea();
   std::vector<bool> discovered(NUM_TILES, false);
   const shapes::Point2D srcTile(src.x / m_movementTileSize, src.y / m_movementTileSize);
   
   openSet.push_back(new AStarPoint(NULL, srcTile.x, srcTile.y, 0, 0));
   std::push_heap(openSet.begin(), openSet.end(), AStarPoint::IsLowerPriority());         

   const int sourceTileNum = pixelsToTileNum(src);
   const int destinationTileNum = pixelsToTileNum(dst);

   discovered[sourceTileNum] = true;

   Path path;
      
   while(!openSet.empty())
   {
      // Get the lowest-cost point in the open set, and remove it from the open set
      const AStarPoint* cheapestPoint = openSet.front();
      std::pop_heap(openSet.begin(), openSet.end(), AStarPoint::IsLowerPriority());
      openSet.pop_back();
      closedSet.push_back(cheapestPoint);

      if(*cheapestPoint == destinationPoint)
      {
         DEBUG("Found goal point %d,%d", cheapestPoint->x, cheapestPoint->y);
         const AStarPoint* curr = cheapestPoint;
         while(curr != NULL)
         {
            path.push_front(shapes::Point2D(curr->x * m_movementTileSize, curr->y * m_movementTileSize));
            curr = curr->getParent();
         }
         break;
      }
      
      DEBUG("Evaluating point %d,%d", cheapestPoint->x, cheapestPoint->y);

      // Evaluate all the existing laterally adjacent points,
      // adding 1 as the cost of reaching the point from our current cheapest point.
      const std::vector<shapes::Point2D> lateralPoints = shapes::Point2D::getLaterallyAdjacentPoints(*cheapestPoint, m_collisionGridBounds);
      evaluateAdjacentNodes(entityGrid, entityState, lateralPoints, cheapestPoint, 1.0f, destinationTileNum, openSet, discovered, size);

      // Evaluate all the existing diagonally adjacent points,
      // adding the square root of 2 as the cost of reaching the point from our current cheapest point.
      const std::vector<shapes::Point2D> diagonalPoints = shapes::Point2D::getDiagonallyAdjacentPoints(*cheapestPoint, m_collisionGridBounds);
      evaluateAdjacentNodes(entityGrid, entityState, diagonalPoints, cheapestPoint, ROOT_2, destinationTileNum, openSet, discovered, size, true);
   }
   
   for(std::vector<AStarPoint*>::const_iterator iter = openSet.begin(); iter != openSet.end(); ++iter)
   {
      delete *iter;
   }
   
   for(std::vector<const AStarPoint*>::const_iterator iter = closedSet.begin(); iter != closedSet.end(); ++iter)
   {
      delete *iter;
   }
   
   return path;
}

void Pathfinder::evaluateAdjacentNodes(const EntityGrid& entityGrid, const TileState& entityState, const std::vector<shapes::Point2D>& adjacentNodes, const AStarPoint* evaluatedPoint, float traversalCost, int destinationTileNum, std::vector<AStarPoint*>& openSet, std::vector<bool>& discovered, const shapes::Size& size, bool diagonalMovement) const
{
   for(std::vector<shapes::Point2D>::const_iterator iter = adjacentNodes.begin(); iter != adjacentNodes.end(); ++iter)
   {
      int adjacentTileNum = coordsToTileNum(*iter);
      float tileGCost = evaluatedPoint->getGCost() + traversalCost;
      float tileHCost = m_distanceMatrix[adjacentTileNum][destinationTileNum];
      if(!discovered[adjacentTileNum])
      {
         discovered[adjacentTileNum] = true;
         const int x = iter->x;
         const int y = iter->y;
         
         bool freeTile = entityGrid.canOccupyArea(shapes::Rectangle(shapes::Point2D(x, y) * m_movementTileSize, size), entityState);

         if(diagonalMovement)
         {
            freeTile = freeTile && entityGrid.canOccupyArea(shapes::Rectangle(shapes::Point2D(evaluatedPoint->x, y), size), entityState)
            && entityGrid.canOccupyArea(shapes::Rectangle(shapes::Point2D(x, evaluatedPoint->y), size), entityState);
         }

         if(freeTile)
         {
            DEBUG("Pushing point %d,%d onto open set with g()=%f and f()=%f.", iter->x, iter->y, tileGCost, tileGCost + tileHCost);
            openSet.push_back(new AStarPoint(evaluatedPoint, iter->x, iter->y, tileGCost, tileHCost));
            std::push_heap(openSet.begin(), openSet.end(), AStarPoint::IsLowerPriority());
         }
      }
      else
      {
         AStarPoint::ArePointsEqual equality = { *iter };
         std::vector<AStarPoint*>::const_iterator tileInOpenSet = std::find_if(openSet.begin(), openSet.end(), equality);
         if(tileInOpenSet != openSet.end() && (*tileInOpenSet)->getGCost() > tileGCost)
         {
            DEBUG("Altering cost of discovered point %d, %d to g()=%f and f()=%f", iter->x, iter->y, tileGCost, tileGCost + tileHCost);
            (*tileInOpenSet)->setGCost(tileGCost);
            (*tileInOpenSet)->setParent(evaluatedPoint);
            std::make_heap(openSet.begin(), openSet.end(), AStarPoint::IsLowerPriority());
         }
      }
   }
}

Pathfinder::Path Pathfinder::findRFWPath(const shapes::Point2D& src, const shapes::Point2D& dst) const
{
   Path path;

   int srcTileNum = pixelsToTileNum(src);
   const int dstTileNum = pixelsToTileNum(dst);

   for(;;)
   {
      int nextTile = m_successorMatrix[srcTileNum][dstTileNum];
      if(nextTile == -1)
      {
         break;
      }
      
      path.push_back(tileNumToPixels(nextTile));
      srcTileNum = nextTile;
   }

   return path;
}

void Pathfinder::deleteRoyFloydWarshallMatrices()
{
   const unsigned int NUM_TILES = m_collisionGridBounds.getArea();
   if(m_distanceMatrix)
   {
      for(unsigned int i = 0; i < NUM_TILES; ++i)
      {
         delete [] m_distanceMatrix[i];
      }

      delete [] m_distanceMatrix;
      m_distanceMatrix = NULL;
   }

   if(m_successorMatrix)
   {
      for(unsigned int i = 0; i < NUM_TILES; ++i)
      {
         delete [] m_successorMatrix[i];
      }

      delete [] m_successorMatrix;
      m_successorMatrix = NULL;
   }
}
