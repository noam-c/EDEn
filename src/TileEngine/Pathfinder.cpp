/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
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

shapes::Point2D Pathfinder::tileNumToCoords(int tileNum)
{
   div_t result = div(tileNum, collisionGridBounds.getWidth());
   return shapes::Point2D(result.rem, result.quot);
}

shapes::Point2D Pathfinder::tileNumToPixels(int tileNum)
{
   shapes::Point2D p = tileNumToCoords(tileNum);
   return p * movementTileSize;
}

int Pathfinder::coordsToTileNum(const shapes::Point2D& tileLocation)
{
   return (tileLocation.y * collisionGridBounds.getWidth() + tileLocation.x);
}

int Pathfinder::pixelsToTileNum(const shapes::Point2D& pixelLocation)
{
   return coordsToTileNum(pixelLocation / movementTileSize);
}

Pathfinder::Pathfinder() : distanceMatrix(NULL), successorMatrix(NULL), collisionGrid(NULL)
{
}

void Pathfinder::initialize(TileState** grid, int tileSize, const shapes::Rectangle& gridBounds)
{
   DEBUG("Resetting pathfinder...");
   deleteRoyFloydWarshallMatrices();
   movementTileSize = tileSize;
   collisionGrid = grid;
   collisionGridBounds = gridBounds;
   initRoyFloydWarshallMatrices();
   DEBUG("Pathfinder reinitialized.");
}

void Pathfinder::initRoyFloydWarshallMatrices()
{
   const unsigned int NUM_TILES = collisionGridBounds.getArea();
   
   distanceMatrix = new float*[NUM_TILES];
   successorMatrix = new int*[NUM_TILES];

   for(unsigned int i = 0; i < NUM_TILES; ++i)
   {
      distanceMatrix[i] = new float[NUM_TILES];
      successorMatrix[i] = new int[NUM_TILES];
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
            distanceMatrix[a][b] = 0;
            successorMatrix[a][b] = -1;
         }
         else
         {
            bTile = tileNumToCoords(b);

            bool xAdjacent = aTile.x - 1 <= bTile.x && bTile.x <= aTile.x + 1;
            bool yAdjacent = aTile.y - 1 <= bTile.y && bTile.y <= aTile.y + 1;
            
            bool adjacent = xAdjacent && yAdjacent;
            
            if(!adjacent 
               || collisionGrid[aTile.y][aTile.x].entityType == TileState::OBSTACLE 
               || collisionGrid[bTile.y][bTile.x].entityType == TileState::OBSTACLE)
            {
               distanceMatrix[a][b] = INFINITY;
               successorMatrix[a][b] = -1;
            }
            else
            {
               successorMatrix[a][b] = b;
               if(aTile.x != bTile.x && aTile.y != bTile.y)
               {
                  distanceMatrix[a][b] = ROOT_2;
               }
               else
               {
                  distanceMatrix[a][b] = 1;
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
            float distance = distanceMatrix[a][i] + distanceMatrix[i][b];
            if(distance < distanceMatrix[a][b])
            {
               distanceMatrix[a][b] = distance;
               successorMatrix[a][b] = successorMatrix[a][i];
            }
         }
      }
   }
}

Pathfinder::Path Pathfinder::findBestPath(const shapes::Point2D& src, const shapes::Point2D& dst)
{
   return findRFWPath(src, dst);
}

Pathfinder::Path Pathfinder::findReroutedPath(const EntityGrid& entityGrid, const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size)
{
   return findAStarPath(entityGrid, src, dst, size);
}

class Pathfinder::AStarPoint : public shapes::Point2D
{
   private:
      const AStarPoint* parent;
      
      float gCost;
      float hCost;

      float fCost;

   public:
      AStarPoint(const AStarPoint* parent, int x, int y, float gCost, float hCost) : shapes::Point2D(x,y), parent(parent), gCost(gCost), hCost(hCost), fCost(gCost + hCost) {}

      void setParent(const AStarPoint* newParent)
      {
         parent = newParent;
      }
   
      void setGCost(float newCost)
      {
         gCost = newCost;
         fCost = gCost + hCost;
      }
      
      void setHCost(float newCost)
      {
         hCost = newCost;
         fCost = gCost + hCost;
      }
      
      const AStarPoint* getParent() const
      {
         return parent;
      }
   
      float getGCost() const
      {
         return gCost;
      }
      
      float getHCost() const
      {
         return hCost;
      }

      float getFCost() const
      {
         return fCost;
      }
   
      struct IsLowerPriority
      {
         bool operator()(const AStarPoint* lhs, const AStarPoint* rhs) const
         {
            // We consider lhs to have a lower priority if it has a higher total f() cost.
            // In case of a tie, this point will have lower priority if it has a lower g() cost,
            // indicating that it is not as deep in the search tree.
            return lhs->fCost > rhs->fCost || (lhs->fCost == rhs->fCost && lhs->gCost < rhs->gCost);
         }
      };
   
      struct ArePointsEqual
      {
         const shapes::Point2D& point;
         
         bool operator()(const AStarPoint* other) const
         {
            return point == *other;
         }
      };
};

Pathfinder::Path Pathfinder::findAStarPath(const EntityGrid& entityGrid, const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size)
{
   if(collisionGrid == NULL) return Path();

   const TileState& entityState = collisionGrid[src.y / movementTileSize][src.x / movementTileSize];

   if(!entityGrid.canOccupyArea(shapes::Rectangle(dst, size), entityState)) return Path();

   shapes::Point2D destinationPoint(dst.x / movementTileSize, dst.y / movementTileSize);

   std::vector<AStarPoint*> openSet;
   std::vector<const AStarPoint*> closedSet;
   
   const int NUM_TILES = collisionGridBounds.getArea();
   std::vector<bool> discovered(NUM_TILES, false);
   const shapes::Point2D srcTile(src.x / movementTileSize, src.y / movementTileSize);
   
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
            path.push_front(shapes::Point2D(curr->x * movementTileSize, curr->y * movementTileSize));
            curr = curr->getParent();
         }
         break;
      }
      
      DEBUG("Evaluating point %d,%d", cheapestPoint->x, cheapestPoint->y);

      // Evaluate all the existing laterally adjacent points,
      // adding 1 as the cost of reaching the point from our current cheapest point.
      const std::vector<shapes::Point2D> lateralPoints = shapes::Point2D::getLaterallyAdjacentPoints(*cheapestPoint, collisionGridBounds);
      evaluateAdjacentNodes(entityGrid, entityState, lateralPoints, cheapestPoint, 1.0f, destinationTileNum, openSet, discovered, size);

      // Evaluate all the existing diagonally adjacent points,
      // adding the square root of 2 as the cost of reaching the point from our current cheapest point.
      const std::vector<shapes::Point2D> diagonalPoints = shapes::Point2D::getDiagonallyAdjacentPoints(*cheapestPoint, collisionGridBounds);
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

void Pathfinder::evaluateAdjacentNodes(const EntityGrid& entityGrid, const TileState& entityState, const std::vector<shapes::Point2D>& adjacentNodes, const AStarPoint* evaluatedPoint, float traversalCost, int destinationTileNum, std::vector<AStarPoint*>& openSet, std::vector<bool>& discovered, const shapes::Size& size, bool diagonalMovement)
{
   for(std::vector<shapes::Point2D>::const_iterator iter = adjacentNodes.begin(); iter != adjacentNodes.end(); ++iter)
   {
      int adjacentTileNum = coordsToTileNum(*iter);
      float tileGCost = evaluatedPoint->getGCost() + traversalCost;
      float tileHCost = distanceMatrix[adjacentTileNum][destinationTileNum];
      if(!discovered[adjacentTileNum])
      {
         discovered[adjacentTileNum] = true;
         const int x = iter->x;
         const int y = iter->y;
         
         bool freeTile = entityGrid.canOccupyArea(shapes::Rectangle(shapes::Point2D(x, y) * movementTileSize, size), entityState);

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

Pathfinder::Path Pathfinder::findRFWPath(const shapes::Point2D& src, const shapes::Point2D& dst)
{
   Path path;

   int srcTileNum = pixelsToTileNum(src);
   const int dstTileNum = pixelsToTileNum(dst);

   for(;;)
   {
      int nextTile = successorMatrix[srcTileNum][dstTileNum];
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
   const unsigned int NUM_TILES = collisionGridBounds.getArea();
   if(distanceMatrix)
   {
      for(unsigned int i = 0; i < NUM_TILES; ++i)
      {
         delete [] distanceMatrix[i];
      }

      delete [] distanceMatrix;
      distanceMatrix = NULL;
   }

   if(successorMatrix)
   {
      for(unsigned int i = 0; i < NUM_TILES; ++i)
      {
         delete [] successorMatrix[i];
      }

      delete [] successorMatrix;
      successorMatrix = NULL;
   }
}

Pathfinder::~Pathfinder()
{
   deleteRoyFloydWarshallMatrices();
}
