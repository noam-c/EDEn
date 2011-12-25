/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Pathfinder.h"
#include "Map.h"
#include "XMap.h"
#include "Obstacle.h"
#include "TileEngine.h"
#include "Point2D.h"
#include "GLInclude.h"
#include "stdlib.h"
#include <limits>
#include <algorithm>

#include "DebugUtils.h"
const int debugFlag = DEBUG_PATHFINDER;

//#define DRAW_PATHFINDER

// Movement tile size can be set to a divisor of drawn tile size to increase the pathfinding graph size
// For now, no need for the additional granularity
const int Pathfinder::MOVEMENT_TILE_SIZE = 32;

const float Pathfinder::ROOT_2 = 1.41421356f;
const float Pathfinder::INFINITY = std::numeric_limits<float>::infinity();

Pathfinder::Pathfinder() : map(NULL), distanceMatrix(NULL)
{
}

const Map* Pathfinder::getMapData() const
{
   return map;
}

void Pathfinder::setMapData(const Map* newMapData)
{
   if(map != NULL)
   {
      deleteRoyFloydWarshallMatrices();
      deleteCollisionMap();
   }

   map = newMapData;   
   if(map == NULL) return;

   const int collisionTileRatio = TileEngine::TILE_SIZE / MOVEMENT_TILE_SIZE;
   collisionMapWidth = map->getWidth() * collisionTileRatio;
   collisionMapHeight = map->getHeight() * collisionTileRatio;

   bool** passibilityMap = map->getPassibilityMatrix();
   collisionMap = new TileState*[collisionMapHeight];
   for(int y = 0; y < map->getHeight(); ++y)
   {
      TileState* row = collisionMap[y] = new TileState[collisionMapWidth];
      for(int x = 0; x < map->getWidth(); ++x)
      {
         int xOffset = x * collisionTileRatio;
         bool passible = passibilityMap[y][x];
         for(int i = 0; i < collisionTileRatio; ++i)
         {
            row[i + xOffset].occupantType = passible ? FREE : OBSTACLE;
         }
      }
   }

   std::vector<Obstacle*> obstacles = map->getObstacles();
   std::vector<Obstacle*>::const_iterator iter;
   for(iter = obstacles.begin(); iter != obstacles.end(); ++iter)
   {
      Obstacle* o = *iter;
      addObstacle(Point2D(o->getTileX() * MOVEMENT_TILE_SIZE, o->getTileY() * MOVEMENT_TILE_SIZE), o->getWidth(), o->getHeight());
   }

   initRoyFloydWarshallMatrices();
}

std::string Pathfinder::getName() const
{
   if(map) return map->getName();
   
   T_T("Requested map name when map does not exist.");
}

int Pathfinder::getWidth() const
{
   if(map) return map->getWidth();
   
   T_T("Requested map width when map does not exist.");
}

int Pathfinder::getHeight() const
{
   if(map) return map->getHeight();
   
   T_T("Requested map height when map does not exist.");
}

void Pathfinder::step(long timePassed)
{
   if(map) map->step(timePassed);
}

Point2D Pathfinder::tileNumToCoords(int tileNum)
{
   div_t result = div(tileNum, collisionMapWidth);
   return Point2D(result.rem, result.quot);
}

Point2D Pathfinder::tileNumToPixels(int tileNum)
{
   Point2D p = tileNumToCoords(tileNum);
   p.x *= MOVEMENT_TILE_SIZE;
   p.y *= MOVEMENT_TILE_SIZE;
   return p;
}

int Pathfinder::coordsToTileNum(Point2D tileLocation)
{
   return (tileLocation.y * collisionMapWidth + tileLocation.x);
}

int Pathfinder::pixelsToTileNum(Point2D pixelLocation)
{
   pixelLocation.x /= MOVEMENT_TILE_SIZE;
   pixelLocation.y /= MOVEMENT_TILE_SIZE;
   return coordsToTileNum(pixelLocation);
}

void Pathfinder::initRoyFloydWarshallMatrices()
{
   const int NUM_TILES = collisionMapWidth*collisionMapHeight;
   
   distanceMatrix = new float*[NUM_TILES];
   successorMatrix = new int*[NUM_TILES];

   for(int i = 0; i < NUM_TILES; ++i)
   {
      distanceMatrix[i] = new float[NUM_TILES];
      successorMatrix[i] = new int[NUM_TILES];
   }

   Point2D aTile;
   Point2D bTile;

   for(int a = 0; a < NUM_TILES; ++a)
   {
      aTile = tileNumToCoords(a);
      for(int b = 0; b < NUM_TILES; ++b)
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
               || collisionMap[aTile.y][aTile.x].occupantType == OBSTACLE 
               || collisionMap[bTile.y][bTile.x].occupantType == OBSTACLE)
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

   for(int i = 0; i < NUM_TILES; ++i)
   {
      for(int a = 0; a < NUM_TILES; ++a)
      {
         for(int b = 0; b < NUM_TILES; ++b)
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

Pathfinder::Path Pathfinder::findBestPath(Point2D src, Point2D dst)
{
   return findRFWPath(src, dst);
}

Pathfinder::Path Pathfinder::findReroutedPath(Point2D src, Point2D dst)
{
   return findAStarPath(src, dst);
}

Pathfinder::Path Pathfinder::getStraightPath(Point2D src, Point2D dst)
{
   Path path;
   while(src.x > dst.x + MOVEMENT_TILE_SIZE)
   {
      src.x -= MOVEMENT_TILE_SIZE;
      path.push_back(src);
   }

   while(src.x < dst.x - MOVEMENT_TILE_SIZE)
   {
      src.x += MOVEMENT_TILE_SIZE;
      path.push_back(src);
   }

   while(src.y > dst.y + MOVEMENT_TILE_SIZE)
   {
      src.y -= MOVEMENT_TILE_SIZE;
      path.push_back(src);
   }

   while(src.y < dst.y - MOVEMENT_TILE_SIZE)
   {
      src.y += MOVEMENT_TILE_SIZE;
      path.push_back(src);
   }

   return path;
}

class Pathfinder::AStarPoint : public Point2D
{
   private:
      const AStarPoint* parent;
      
      float gCost;
      float hCost;

      float fCost;

   public:
      AStarPoint(const AStarPoint* parent, int x, int y, float gCost, float hCost) : Point2D(x,y), parent(parent), gCost(gCost), hCost(hCost), fCost(gCost + hCost) {}

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
         const Point2D& point;
         
         bool operator()(const AStarPoint* other) const
         {
            return point == *other;
         }
      };
};

Pathfinder::Path Pathfinder::findAStarPath(Point2D src, Point2D dst)
{
   Point2D destinationPoint(dst.x / MOVEMENT_TILE_SIZE, dst.y / MOVEMENT_TILE_SIZE);
   if(collisionMap == NULL || collisionMap[destinationPoint.y][destinationPoint.x].occupantType != FREE) return Path();

   std::vector<AStarPoint*> openSet;
   std::vector<const AStarPoint*> closedSet;
   
   const int NUM_TILES = collisionMapWidth*collisionMapHeight;
   std::vector<bool> discovered(NUM_TILES, false);
   
   openSet.push_back(new AStarPoint(NULL, src.x / MOVEMENT_TILE_SIZE, src.y / MOVEMENT_TILE_SIZE, 0, 0));
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
            path.push_front(Point2D(curr->x * MOVEMENT_TILE_SIZE, curr->y * MOVEMENT_TILE_SIZE));
            curr = curr->getParent();
         }
         break;
      }
      
      DEBUG("Evaluating point %d,%d", cheapestPoint->x, cheapestPoint->y);

      // Evaluate all the existing laterally adjacent points,
      // adding 1 as the cost of reaching the point from our current cheapest point.
      const std::vector<Point2D> lateralPoints = Point2D::getLaterallyAdjacentPoints(*cheapestPoint, collisionMapWidth, collisionMapHeight);
      evaluateAdjacentNodes(lateralPoints, cheapestPoint, 1.0f, destinationTileNum, openSet, discovered);

      // Evaluate all the existing diagonally adjacent points,
      // adding the square root of 2 as the cost of reaching the point from our current cheapest point.
      const std::vector<Point2D> diagonalPoints = Point2D::getDiagonallyAdjacentPoints(*cheapestPoint, collisionMapWidth, collisionMapHeight);
      evaluateAdjacentNodes(diagonalPoints, cheapestPoint, ROOT_2, destinationTileNum, openSet, discovered);
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

void Pathfinder::evaluateAdjacentNodes(const std::vector<Point2D>& adjacentNodes, const AStarPoint* cheapestPoint, float traversalCost, int destinationTileNum, std::vector<AStarPoint*>& openSet, std::vector<bool>& discovered)
{
   for(std::vector<Point2D>::const_iterator iter = adjacentNodes.begin(); iter != adjacentNodes.end(); ++iter)
   {
      int adjacentTileNum = coordsToTileNum(*iter);
      float tileGCost = cheapestPoint->getGCost() + traversalCost;
      float tileHCost = distanceMatrix[adjacentTileNum][destinationTileNum];
      if(!discovered[adjacentTileNum])
      {
         if(collisionMap[iter->y][iter->x].occupantType == FREE)
         {
            DEBUG("Pushing point %d,%d onto open set with g()=%f and f()=%f.", iter->x, iter->y, tileGCost, tileGCost + tileHCost);
            discovered[adjacentTileNum] = true;
            openSet.push_back(new AStarPoint(cheapestPoint, iter->x, iter->y, tileGCost, tileHCost));
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
            (*tileInOpenSet)->setParent(cheapestPoint);
            std::make_heap(openSet.begin(), openSet.end(), AStarPoint::IsLowerPriority());
         }
      }
   }
}

Pathfinder::Path Pathfinder::findRFWPath(Point2D src, Point2D dst)
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
      
      path.push_back(Point2D(tileNumToPixels(nextTile)));
      srcTileNum = nextTile;
   }

   return path;
}

bool Pathfinder::addObstacle(Point2D area, int width, int height)
{
   return occupyArea(area, width, height, TileState(OBSTACLE));
}

bool Pathfinder::addNPC(NPC* npc, Point2D area, int width, int height)
{
   return occupyArea(area, width, height, TileState(CHARACTER, npc));
}

bool Pathfinder::occupyArea(Point2D area, int width, int height, TileState state)
{
   if(collisionMap == NULL || state.occupantType == FREE)
   {
      return false;
   }

   int collisionMapLeft = area.x/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (area.x + width)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = area.y/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (area.y + height)/MOVEMENT_TILE_SIZE;

   DEBUG("Occupying tiles from %d,%d to %d,%d", collisionMapLeft, collisionMapTop, collisionMapRight, collisionMapBottom);

   for(int collisionMapX = collisionMapLeft; collisionMapX < collisionMapRight; ++collisionMapX)
   {
      for(int collisionMapY = collisionMapTop; collisionMapY <  collisionMapBottom; ++collisionMapY)
      {
         // We cannot occupy the point if it is reserved by an entity other than the occupant attempting to occupy it.
         // For instance, we cannot occupy a tile already occupied by an obstacle or a different character.
         const TileState& collisionTile = collisionMap[collisionMapY][collisionMapX].occupantType;
         if(collisionTile.occupantType == CHARACTER && collisionTile.occupant != state.occupant)
         {
            return false;
         }
         else if(collisionTile.occupantType != FREE)
         {
            return false;
         }
      }
   }

   setArea(collisionMapLeft, collisionMapTop, collisionMapRight, collisionMapBottom, state);

   return true;
}

void Pathfinder::freeArea(Point2D area, int width, int height)
{
   int collisionMapLeft = area.x/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (area.x + width)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = area.y/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (area.y + height)/MOVEMENT_TILE_SIZE;
   
   DEBUG("Freeing tiles from %d,%d to %d,%d", collisionMapLeft, collisionMapTop, collisionMapRight, collisionMapBottom);
   
   setArea(collisionMapLeft, collisionMapTop, collisionMapRight, collisionMapBottom, TileState(FREE));
}

bool Pathfinder::isAreaFree(Point2D area, int width, int height) const
{
   if(collisionMap == NULL) return false;

   int collisionMapLeft = area.x/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (area.x + width)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = area.y/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (area.y + height)/MOVEMENT_TILE_SIZE;
   
   for(int collisionMapX = collisionMapLeft; collisionMapX < collisionMapRight; ++collisionMapX)
   {
      for(int collisionMapY = collisionMapTop; collisionMapY < collisionMapBottom; ++collisionMapY)
      {
         // We cannot occupy the point if it is reserved by an obstacle or a character.
         const TileState& collisionTile = collisionMap[collisionMapY][collisionMapX].occupantType;
         if(collisionTile.occupantType != FREE)
         {
            return false;
         }
      }
   }
   
   return true;
}

bool Pathfinder::beginMovement(NPC* npc, Point2D src, Point2D dst, int width, int height)
{
   return occupyArea(dst, width, height, TileState(CHARACTER, npc));
}

void Pathfinder::endMovement(Point2D src, Point2D dst, int width, int height)
{
   freeArea(src, width, height);
}

void Pathfinder::setArea(int left, int top, int right, int bottom, TileState state)
{
   for(int collisionMapX = left; collisionMapX < right; ++collisionMapX)
   {
      for(int collisionMapY = top; collisionMapY < bottom; ++collisionMapY)
      {
         collisionMap[collisionMapY][collisionMapX] = state;
      }
   }
}

void Pathfinder::draw()
{
   if(map == NULL) return;

#ifndef DRAW_PATHFINDER
   map->draw();
#else
   for(int y = 0; y < collisionMapHeight; ++y)
   {
      for(int x = 0; x < collisionMapWidth; ++x)
      {
         float destLeft = float(x * MOVEMENT_TILE_SIZE);
         float destRight = float((x + 1) * MOVEMENT_TILE_SIZE);
         float destTop = float(y * MOVEMENT_TILE_SIZE);
         float destBottom = float((y + 1) * MOVEMENT_TILE_SIZE);
         
         glDisable(GL_TEXTURE_2D);
         glBegin(GL_QUADS);
         
         switch(collisionMap[y][x].occupantType)
         {
            case FREE:
            {
               glColor3f(0.0f, 0.5f, 0.0f);
               break;
            }
            case CHARACTER:
            {
               glColor3f(0.0f, 0.0f, 0.5f);
               break;
            }
            case OBSTACLE:
            default:
            {
               glColor3f(0.5f, 0.5f, 0.0f);
               break;
            }
         }
         
         glVertex3f(destLeft, destTop, 0.0f);
         glVertex3f(destRight, destTop, 0.0f);
         glVertex3f(destRight, destBottom, 0.0f);
         glVertex3f(destLeft, destBottom, 0.0f);
         glColor3f(1.0f, 1.0f, 1.0f);
         glEnd();
         glEnable(GL_TEXTURE_2D);
      }
   }
#endif
}

void Pathfinder::deleteRoyFloydWarshallMatrices()
{
   const int numTiles = collisionMapWidth * collisionMapHeight;
   if(distanceMatrix)
   {
      for(int i = 0; i < numTiles; ++i)
      {
         delete [] distanceMatrix[i];
      }

      delete [] distanceMatrix;
      distanceMatrix = NULL;
   }

   if(successorMatrix)
   {
      for(int i = 0; i < numTiles; ++i)
      {
         delete [] successorMatrix[i];
      }

      delete [] successorMatrix;
      successorMatrix = NULL;
   }
}

void Pathfinder::deleteCollisionMap()
{
   if(collisionMap)
   {
      for(int i = 0; i < collisionMapHeight; ++i)
      {
         delete [] collisionMap[i];
      }

      delete [] collisionMap;
      collisionMap = NULL;
   }
}

Pathfinder::~Pathfinder()
{
   deleteRoyFloydWarshallMatrices();
   deleteCollisionMap();
}
