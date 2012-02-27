/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "EntityGrid.h"
#include "Map.h"
#include "XMap.h"
#include "Obstacle.h"
#include "TileEngine.h"
#include "TileState.h"
#include "Rectangle.h"
#include "Actor.h"
#include "GLInclude.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_ENTITY_GRID;

//#define DRAW_ENTITY_GRID

// Movement tile size can be set to a divisor of drawn tile size to increase the pathfinding graph size
// For now, no need for the additional granularity
const int EntityGrid::MOVEMENT_TILE_SIZE = 16;

const float EntityGrid::ROOT_2 = 1.41421356f;
const float EntityGrid::INFINITY = std::numeric_limits<float>::infinity();

EntityGrid::EntityGrid() : map(NULL), collisionMap(NULL)
{
}

Rectangle EntityGrid::getCollisionMapEdges(const Rectangle& area) const
{
   int collisionMapLeft = area.left/MOVEMENT_TILE_SIZE;
   int collisionMapRight = (area.right - 1)/MOVEMENT_TILE_SIZE;
   int collisionMapTop = area.top/MOVEMENT_TILE_SIZE;
   int collisionMapBottom = (area.bottom - 1)/MOVEMENT_TILE_SIZE;

   return Rectangle(collisionMapTop, collisionMapLeft, collisionMapBottom, collisionMapRight);
}

const Map* EntityGrid::getMapData() const
{
   return map;
}

void EntityGrid::setMapData(const Map* newMapData)
{
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
         row[x].entityType = passible ? TileState::FREE : TileState::OBSTACLE;
         row[x].entity = NULL;
      }
   }

   std::vector<Obstacle*> obstacles = map->getObstacles();
   std::vector<Obstacle*>::const_iterator iter;
   for(iter = obstacles.begin(); iter != obstacles.end(); ++iter)
   {
      Obstacle* o = *iter;
      addObstacle(Point2D(o->getTileX() * MOVEMENT_TILE_SIZE, o->getTileY() * MOVEMENT_TILE_SIZE), o->getWidth(), o->getHeight());
   }

   pathfinder.initialize(collisionMap, MOVEMENT_TILE_SIZE, collisionMapWidth, collisionMapHeight);
}

std::string EntityGrid::getName() const
{
   if(map) return map->getName();
   
   T_T("Requested map name when map does not exist.");
}

int EntityGrid::getWidth() const
{
   if(map) return map->getWidth();
   
   T_T("Requested map width when map does not exist.");
}

int EntityGrid::getHeight() const
{
   if(map) return map->getHeight();
   
   T_T("Requested map height when map does not exist.");
}

bool EntityGrid::withinMap(const Point2D& point) const
{
   return withinMap(point.x, point.y);
}

bool EntityGrid::withinMap(const int x, const int y) const
{
   return map != NULL && x >= 0 && x < getWidth() * TileEngine::TILE_SIZE && y >= 0 && y < getHeight() * TileEngine::TILE_SIZE;
}

void EntityGrid::step(long timePassed)
{
   if(map) map->step(timePassed);
}

EntityGrid::Path EntityGrid::findBestPath(const Point2D& src, const Point2D& dst)
{
   return pathfinder.findBestPath(src, dst);
}

EntityGrid::Path EntityGrid::findReroutedPath(const Point2D& src, const Point2D& dst, int width, int height)
{
   return pathfinder.findReroutedPath(*this, src, dst, width, height);
}

bool EntityGrid::addObstacle(const Point2D& area, int width, int height)
{
   return occupyArea(area, width, height, TileState(TileState::OBSTACLE));
}

bool EntityGrid::addActor(Actor* actor, const Point2D& area, int width, int height)
{
   return occupyArea(area, width, height, TileState(TileState::ACTOR, actor));
}

bool EntityGrid::changeActorLocation(Actor* actor, const Point2D& src, const Point2D& dst, int width, int height)
{
   TileState actorState(TileState::ACTOR, actor);
   if(occupyArea(dst, width, height, actorState))
   {
      freeArea(src, dst, width, height, actorState);
      return true;
   }

   return false;
}

void EntityGrid::removeActor(Actor* actor, const Point2D& currentLocation, int width, int height)
{
   freeArea(currentLocation, width, height);
}

Actor* EntityGrid::getAdjacentActor(Actor* actor) const
{
   if(collisionMap == NULL)
   {
      return NULL;
   }
   
   Point2D adjacentLocation = actor->getLocation();
   const int width = actor->getWidth();
   const int height = actor->getHeight();
   const MovementDirection direction = actor->getDirection();
   switch(direction)
   {
      case UP_LEFT:
      case LEFT:
      case DOWN_LEFT:
      {
         adjacentLocation.x -= MOVEMENT_TILE_SIZE;
         break;
      }
      case UP_RIGHT:
      case RIGHT:
      case DOWN_RIGHT:
      {
         adjacentLocation.x += width;
         break;
      }
      default:
      {
         break;
      }   
   }
   
   switch(direction)
   {
      case UP_RIGHT:
      case UP:
      case UP_LEFT:
      {
         adjacentLocation.y -= MOVEMENT_TILE_SIZE;
         break;
      }
      case DOWN_LEFT:
      case DOWN:
      case DOWN_RIGHT:
      {
         adjacentLocation.y += height;
         break;
      }
      default:
      {
         break;
      }
   }

   int rectLeft = std::max(0, adjacentLocation.x/MOVEMENT_TILE_SIZE);
   int rectRight = std::min(collisionMapWidth, (adjacentLocation.x + width - 1)/MOVEMENT_TILE_SIZE);
   int rectTop = std::max(0, adjacentLocation.y/MOVEMENT_TILE_SIZE);
   int rectBottom = std::min(collisionMapHeight, (adjacentLocation.y + height - 1)/MOVEMENT_TILE_SIZE);
   
   for(int rectY = rectTop; rectY <= rectBottom; ++rectY)
   {
      for(int rectX = rectLeft; rectX <= rectRight; ++rectX)
      {
         const TileState& collisionTile = collisionMap[rectY][rectX];
         if(collisionTile.entityType == TileState::ACTOR && collisionTile.entity != actor)
         {
            return static_cast<Actor*>(collisionTile.entity);
         }
      }
   }
   
   return NULL;
}

bool EntityGrid::canOccupyArea(const Point2D& area, int width, int height, TileState state) const
{
   if(collisionMap == NULL || state.entityType == TileState::FREE)
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
         // We cannot occupy the point if it is reserved by an entity other than the entity attempting to occupy it.
         // For instance, we cannot occupy a tile already occupied by an obstacle or a different character.
         const TileState& collisionTile = collisionMap[collisionMapY][collisionMapX];
         if(collisionTile.entityType != TileState::FREE)
         {
            if(collisionTile.entityType != state.entityType || collisionTile.entity != state.entity)
            {
               return false;
            } 
         }
      }
   }

   return true;
}

bool EntityGrid::occupyArea(const Point2D& area, int width, int height, TileState state)
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

void EntityGrid::freeArea(const Point2D& locationToFree, int width, int height)
{
   Rectangle rectToFree = getCollisionMapEdges(Rectangle(locationToFree, width, height));
   
   DEBUG("Freeing tiles from %d,%d to %d,%d", rectToFree.left, rectToFree.top, rectToFree.right, rectToFree.bottom);
   
   setArea(rectToFree, TileState(TileState::FREE));
}

void EntityGrid::freeArea(const Point2D& previousLocation, const Point2D& currentLocation, int width, int height, TileState state)
{
   freeArea(previousLocation, width, height);

   Rectangle currentRect = getCollisionMapEdges(Rectangle(currentLocation, width, height));
   setArea(currentRect, state);
}

bool EntityGrid::isAreaFree(const Point2D& area, int width, int height) const
{
   if(collisionMap == NULL) return false;

   Rectangle areaRect = getCollisionMapEdges(Rectangle(area, width, height));

   for(int collisionMapY = areaRect.top; collisionMapY < areaRect.bottom; ++collisionMapY)
   {
      for(int collisionMapX = areaRect.left; collisionMapX < areaRect.right; ++collisionMapX)
      {
         // We cannot occupy the point if it is reserved by an obstacle or a character.
         const TileState& collisionTile = collisionMap[collisionMapY][collisionMapX];
         if(collisionTile.entityType != TileState::FREE)
         {
            return false;
         }
      }
   }
   
   return true;
}

void EntityGrid::moveToClosestPoint(Actor* actor, int width, int height, int xDirection, int yDirection, int distance)
{
   if(xDirection == 0 && yDirection == 0) return;
   if(distance == 0) return;

   TileState playerState(TileState::ACTOR, actor);

   const Point2D source = actor->getLocation();
   
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

      if(!canOccupyArea(nextPoint, width, height, playerState))
      {
         break;
      }

      lastAvailablePoint = nextPoint;      
   }

   if(lastAvailablePoint != source)
   {
      if(!occupyArea(lastAvailablePoint, width, height, playerState))
      {
         // If updating failed, just stick with the start location
         occupyArea(source, width, height, playerState);
      }
      else
      {
         // If we moved, update the map accordingly
         freeArea(source, lastAvailablePoint, width, height, playerState);

         actor->setLocation(lastAvailablePoint);
      }
   }
}

bool EntityGrid::beginMovement(Actor* actor, const Point2D& src, const Point2D& dst, int width, int height)
{
   return occupyArea(dst, width, height, TileState(TileState::ACTOR, actor));
}

void EntityGrid::abortMovement(Actor* actor, const Point2D& src, const Point2D& dst, const Point2D& currentLocation, int width, int height)
{
   freeArea(src, currentLocation, width, height, TileState(TileState::ACTOR, actor));
   freeArea(dst, currentLocation, width, height, TileState(TileState::ACTOR, actor));
}

void EntityGrid::endMovement(Actor* actor, const Point2D& src, const Point2D& dst, int width, int height)
{
   freeArea(src, dst, width, height, TileState(TileState::ACTOR, actor));
}

void EntityGrid::setArea(const Rectangle& area, TileState state)
{
   if(collisionMap == NULL) return;

   for(int collisionMapY = area.top; collisionMapY <= area.bottom; ++collisionMapY)
   {
      for(int collisionMapX = area.left; collisionMapX <= area.right; ++collisionMapX)
      {
         collisionMap[collisionMapY][collisionMapX] = state;
      }
   }
}

Actor* EntityGrid::getEntityActor(const Point2D& location, int width, int height) const
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
         if(collisionTile.entityType == TileState::ACTOR)
         {
            return reinterpret_cast<Actor*>(collisionTile.entity);
         }
      }
   }

   return NULL;
}

void EntityGrid::draw()
{
   if(map == NULL) return;

#ifndef DRAW_ENTITY_GRID
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
         
         switch(collisionMap[y][x].entityType)
         {
            case TileState::FREE:
            {
               glColor3f(0.0f, 0.5f, 0.0f);
               break;
            }
            case NPC_CHARACTER:
            {
               if(collisionMap[y][x].entity == NULL)
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
            case TileState::OBSTACLE:
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

void EntityGrid::deleteCollisionMap()
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

EntityGrid::~EntityGrid()
{
   deleteCollisionMap();
}
