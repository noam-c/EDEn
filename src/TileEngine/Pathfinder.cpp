/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Pathfinder.h"
#include "Map.h"
#include "XMap.h"
#include "Obstacle.h"
#include "TileEngine.h"
#include "Point2D.h"
#include "Rectangle.h"
#include "PlayerCharacter.h"
#include "GLInclude.h"
#include "stdlib.h"
#include <limits>
#include <algorithm>

#include "DebugUtils.h"
const int debugFlag = DEBUG_PATHFINDER;

#define DRAW_PATHFINDER

// Movement tile size can be set to a divisor of drawn tile size to increase the pathfinding graph size
// For now, no need for the additional granularity
const int Pathfinder::MOVEMENT_TILE_SIZE = 16;

const float Pathfinder::ROOT_2 = 1.41421356f;
const float Pathfinder::INFINITY = std::numeric_limits<float>::infinity();

Pathfinder::TileState::TileState() : occupantType(FREE), occupant(NULL)
{
}

Pathfinder::TileState::TileState(OccupantType type) : occupantType(type), occupant(NULL)
{
}

Pathfinder::TileState::TileState(OccupantType type, void* occupant) : occupantType(type), occupant(occupant)
{
}

Pathfinder::Pathfinder() : map(NULL), collisionMap(NULL), distanceMatrix(NULL), successorMatrix(NULL)
{
}

Rectangle Pathfinder::getCollisionMapEdges(Rectangle area) const
{
   int collisionMapLeft = area.left/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (area.right - 1)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = area.top/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (area.bottom - 1)/MOVEMENT_TILE_SIZE;

   return Rectangle(collisionMapTop, collisionMapLeft, collisionMapBottom, collisionMapRight);
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
   for(int y = 0; y < collisionMapHeight; ++y)
   {
      TileState* row = collisionMap[y] = new TileState[collisionMapWidth];
      for(int x = 0; x < collisionMapWidth; ++x)
      {
         bool passible = passibilityMap[y / collisionTileRatio][x / collisionTileRatio];
         row[x].occupantType = passible ? FREE : OBSTACLE;
         row[x].occupant = NULL;
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

Pathfinder::Path Pathfinder::findReroutedPath(Point2D src, Point2D dst, int width, int height)
{
   return findAStarPath(src, dst, width, height);
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

Pathfinder::Path Pathfinder::findAStarPath(Point2D src, Point2D dst, int width, int height)
{
   Point2D destinationPoint(dst.x / MOVEMENT_TILE_SIZE, dst.y / MOVEMENT_TILE_SIZE);
   if(collisionMap == NULL || collisionMap[destinationPoint.y][destinationPoint.x].occupantType != FREE) return Path();

   std::vector<AStarPoint*> openSet;
   std::vector<const AStarPoint*> closedSet;
   
   const int NUM_TILES = collisionMapWidth*collisionMapHeight;
   std::vector<bool> discovered(NUM_TILES, false);
   
   const int widthTileRatio = width / MOVEMENT_TILE_SIZE;
   const int heightTileRatio = height / MOVEMENT_TILE_SIZE;
   const Point2D srcTile(src.x / MOVEMENT_TILE_SIZE, src.y / MOVEMENT_TILE_SIZE);
   
   for(int xOffset = 0; xOffset < widthTileRatio; ++xOffset)
   {
      for(int yOffset = 0; yOffset < heightTileRatio; ++yOffset)
      {
         openSet.push_back(new AStarPoint(NULL, srcTile.x + xOffset, srcTile.y + yOffset, 0, 0));
         std::push_heap(openSet.begin(), openSet.end(), AStarPoint::IsLowerPriority());         
      }
   }

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
      evaluateAdjacentNodes(lateralPoints, cheapestPoint, 1.0f, destinationTileNum, openSet, discovered, widthTileRatio, heightTileRatio);

      // Evaluate all the existing diagonally adjacent points,
      // adding the square root of 2 as the cost of reaching the point from our current cheapest point.
      const std::vector<Point2D> diagonalPoints = Point2D::getDiagonallyAdjacentPoints(*cheapestPoint, collisionMapWidth, collisionMapHeight);
      evaluateAdjacentNodes(diagonalPoints, cheapestPoint, ROOT_2, destinationTileNum, openSet, discovered, widthTileRatio, heightTileRatio);
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

void Pathfinder::evaluateAdjacentNodes(const std::vector<Point2D>& adjacentNodes, const AStarPoint* cheapestPoint, float traversalCost, int destinationTileNum, std::vector<AStarPoint*>& openSet, std::vector<bool>& discovered, int widthTileRatio, int heightTileRatio)
{
   for(std::vector<Point2D>::const_iterator iter = adjacentNodes.begin(); iter != adjacentNodes.end(); ++iter)
   {
      int adjacentTileNum = coordsToTileNum(*iter);
      float tileGCost = cheapestPoint->getGCost() + traversalCost;
      float tileHCost = distanceMatrix[adjacentTileNum][destinationTileNum];
      if(!discovered[adjacentTileNum])
      {
         const int x = iter->x;
         const int y = iter->y;
         
         if(x + widthTileRatio < collisionMapWidth && y + heightTileRatio < collisionMapHeight)
         {
            bool freeTile = true;
            for(int xOffset = 0; xOffset < widthTileRatio; ++xOffset)
            {
               for(int yOffset = 0; yOffset < heightTileRatio; ++yOffset)
               {
                  if(collisionMap[y + yOffset][x + xOffset].occupantType != FREE)
                  {
                     freeTile = false;
                     break;
                  }
               }
            }
            
            if(freeTile)
            {
               DEBUG("Pushing point %d,%d onto open set with g()=%f and f()=%f.", iter->x, iter->y, tileGCost, tileGCost + tileHCost);
               discovered[adjacentTileNum] = true;
               openSet.push_back(new AStarPoint(cheapestPoint, iter->x, iter->y, tileGCost, tileHCost));
               std::push_heap(openSet.begin(), openSet.end(), AStarPoint::IsLowerPriority());
            }
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
   return occupyArea(area, width, height, TileState(NPC_CHARACTER, npc));
}

bool Pathfinder::addPlayer(PlayerCharacter* player, Point2D area, int width, int height)
{
   return occupyArea(area, width, height, TileState(PLAYER_CHARACTER, player));
}

bool Pathfinder::canOccupyArea(Point2D area, int width, int height, TileState state) const
{
   if(collisionMap == NULL || state.occupantType == FREE)
   {
      return false;
   }
   
   Rectangle areaRect = getCollisionMapEdges(Rectangle(area, width, height));
   
   if(areaRect.right >= collisionMapWidth || areaRect.bottom >= collisionMapHeight)
   {
      return false;
   }
   
   for(int collisionMapY = areaRect.top; collisionMapY <=  areaRect.bottom; ++collisionMapY)
   {
      for(int collisionMapX = areaRect.left; collisionMapX <= areaRect.right; ++collisionMapX)
      {
         // We cannot occupy the point if it is reserved by an entity other than the occupant attempting to occupy it.
         // For instance, we cannot occupy a tile already occupied by an obstacle or a different character.
         const TileState& collisionTile = collisionMap[collisionMapY][collisionMapX];
         if(collisionTile.occupantType != FREE)
         {
            if(collisionTile.occupantType != state.occupantType || collisionTile.occupant != state.occupant)
            {
               return false;
            } 
         }
      }
   }

   return true;
}

bool Pathfinder::occupyArea(Point2D area, int width, int height, TileState state)
{
   Rectangle areaRect = getCollisionMapEdges(Rectangle(area, width, height));

   if(canOccupyArea(area, width, height, state))
   {
      DEBUG("Occupying tiles from %d,%d to %d,%d", areaRect.left, areaRect.top, areaRect.right, areaRect.bottom);
      
      setArea(areaRect, state);
      return true;
   }

   DEBUG("Couldn't occupy tiles from %d,%d to %d,%d", areaRect.left, areaRect.top, areaRect.right, areaRect.bottom);
   return false;
}

void Pathfinder::freeArea(Point2D previousLocation, Point2D currentLocation, int width, int height, TileState state)
{
   Rectangle previousRect = getCollisionMapEdges(Rectangle(previousLocation, width, height));
   Rectangle currentRect = getCollisionMapEdges(Rectangle(currentLocation, width, height));
   
   DEBUG("Freeing tiles from %d,%d to %d,%d", previousRect.left, previousRect.top, previousRect.right, previousRect.bottom);
   
   setArea(previousRect, TileState(FREE));
   setArea(currentRect, state);
}

bool Pathfinder::isAreaFree(Point2D area, int width, int height) const
{
   if(collisionMap == NULL) return false;

   Rectangle areaRect = getCollisionMapEdges(Rectangle(area, width, height));

   for(int collisionMapY = areaRect.top; collisionMapY < areaRect.bottom; ++collisionMapY)
   {
      for(int collisionMapX = areaRect.left; collisionMapX < areaRect.right; ++collisionMapX)
      {
         // We cannot occupy the point if it is reserved by an obstacle or a character.
         const TileState& collisionTile = collisionMap[collisionMapY][collisionMapX];
         if(collisionTile.occupantType != FREE)
         {
            return false;
         }
      }
   }
   
   return true;
}

void Pathfinder::moveToClosestPoint(PlayerCharacter* player, int playerWidth, int playerHeight, int xDirection, int yDirection, int distance)
{
   if(xDirection == 0 && yDirection == 0) return;
   if(distance == 0) return;

   TileState playerState(PLAYER_CHARACTER, player);

   const Point2D source = player->getLocation();
   
   const int mapPixelWidth = (collisionMapWidth - 1) * MOVEMENT_TILE_SIZE;
   const int mapPixelHeight = (collisionMapHeight - 1) * MOVEMENT_TILE_SIZE; 
   
   Point2D lastAvailablePoint = source;
   
   while(distance > 0)
   {
      int distanceTraversed = std::min(distance, MOVEMENT_TILE_SIZE / 2);
      distance -= distanceTraversed;
      
      // Get the next point for movement, and clamp it to the map dimensions
      Point2D nextPoint;
      nextPoint.x = lastAvailablePoint.x + xDirection * distanceTraversed;
      nextPoint.x = std::max(nextPoint.x, 0);
      nextPoint.x = std::min(nextPoint.x, mapPixelWidth - MOVEMENT_TILE_SIZE);

      nextPoint.y = lastAvailablePoint.y + yDirection * distanceTraversed;
      nextPoint.y = std::max(nextPoint.y, 0);
      nextPoint.y = std::min(nextPoint.y, mapPixelHeight - MOVEMENT_TILE_SIZE);

      if(lastAvailablePoint == nextPoint)
      {
         // We haven't moved any further, so we are at a dead end
         break;
      }

      if(!canOccupyArea(nextPoint, playerWidth, playerHeight, playerState))
      {
         break;
      }

      lastAvailablePoint = nextPoint;      
   }
   
   
   if(lastAvailablePoint != source)
   {
      if(!occupyArea(lastAvailablePoint, playerWidth, playerHeight, playerState))
      {
         // If updating failed, just stick with the start location
         occupyArea(source, playerWidth, playerHeight, playerState);
      }
      else
      {
         // If we moved, update the map accordingly
         freeArea(source, lastAvailablePoint, playerWidth, playerHeight, playerState);

         player->setLocation(lastAvailablePoint);
      }
   }
}

bool Pathfinder::beginMovement(NPC* npc, Point2D src, Point2D dst, int width, int height)
{
   return occupyArea(dst, width, height, TileState(NPC_CHARACTER, npc));
}

void Pathfinder::abortMovement(NPC* npc, Point2D src, Point2D dst, Point2D currentLocation, int width, int height)
{
   freeArea(src, currentLocation, width, height, TileState(NPC_CHARACTER, npc));
   freeArea(dst, currentLocation, width, height, TileState(NPC_CHARACTER, npc));
}

void Pathfinder::endMovement(NPC* npc, Point2D src, Point2D dst, int width, int height)
{
   freeArea(src, dst, width, height, TileState(NPC_CHARACTER, npc));
}

void Pathfinder::setArea(const Rectangle& area, TileState state)
{
   for(int collisionMapY = area.top; collisionMapY <= area.bottom; ++collisionMapY)
   {
      for(int collisionMapX = area.left; collisionMapX <= area.right; ++collisionMapX)
      {
         collisionMap[collisionMapY][collisionMapX] = state;
      }
   }
}

NPC* Pathfinder::getOccupantNPC(Point2D location, int width, int height) const
{
   if(collisionMap == NULL)
   {
      return NULL;
   }
   
   int collisionMapLeft = location.x/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (location.x + width - 1)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = location.y/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (location.y + height - 1)/MOVEMENT_TILE_SIZE;
   
   for(int collisionMapY = collisionMapTop; collisionMapY <=  collisionMapBottom; ++collisionMapY)
   {
      for(int collisionMapX = collisionMapLeft; collisionMapX <= collisionMapRight; ++collisionMapX)
      {
         const TileState& collisionTile = collisionMap[collisionMapY][collisionMapX];
         if(collisionTile.occupantType == NPC_CHARACTER)
         {
            return reinterpret_cast<NPC*>(collisionTile.occupant);
         }
      }
   }

   return NULL;
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
            case NPC_CHARACTER:
            {
               if(collisionMap[y][x].occupant == NULL)
               {
                  glColor3f(0.5f, 0.0f, 0.0f);
               }
               else
               {
                  glColor3f(0.0f, 0.0f, 0.5f);
               }
               break;
            }
            case PLAYER_CHARACTER:
            {
               glColor3f(0.6f, 0.6f, 0.85f);
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
