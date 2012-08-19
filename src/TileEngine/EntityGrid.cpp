/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "EntityGrid.h"
#include "Map.h"
#include "TileEngine.h"
#include "TileState.h"
#include "Point2D.h"
#include "Rectangle.h"
#include "Actor.h"
#include "PlayerCharacter.h"
#include "MessagePipe.h"
#include "TriggerZone.h"
#include "MapExit.h"
#include "ActorMoveMessage.h"
#include "MapExitMessage.h"
#include "MapTriggerMessage.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_ENTITY_GRID;

// Define as 1 to draw the entity grid's state instead of the map
#define DRAW_ENTITY_GRID 0

// Movement tile size can be set to a divisor of drawn tile size to increase the pathfinding graph size
// For now, no need for the additional granularity
const int EntityGrid::MOVEMENT_TILE_SIZE = 32;

const float EntityGrid::ROOT_2 = 1.41421356f;
const float EntityGrid::INFINITY = std::numeric_limits<float>::infinity();

EntityGrid::EntityGrid(const TileEngine& tileEngine, messaging::MessagePipe& messagePipe)
   : tileEngine(tileEngine), messagePipe(messagePipe), map(NULL), collisionMap(NULL)
{
   messagePipe.registerListener(this);
}

shapes::Rectangle EntityGrid::getCollisionMapEdges(const shapes::Rectangle& area) const
{
   const shapes::Point2D collisionMapTopLeft(area.left, area.top);
   const shapes::Point2D collisionMapBottomRight(area.right - 1, area.bottom - 1);

   return shapes::Rectangle(collisionMapTopLeft / MOVEMENT_TILE_SIZE, collisionMapBottomRight / MOVEMENT_TILE_SIZE);
}

const Map* EntityGrid::getMapData() const
{
   return map;
}

void EntityGrid::setMapData(const Map* newMapData)
{
   DEBUG("Resetting entity grid...");
   clearMap();
   map = newMapData;
   if(map == NULL) return;

   const int collisionTileRatio = TileEngine::TILE_SIZE / MOVEMENT_TILE_SIZE;
   collisionMapBounds = shapes::Rectangle(shapes::Point2D::ORIGIN, map->getBounds().getSize() * collisionTileRatio);

   const unsigned int collisionMapHeight = collisionMapBounds.getHeight();
   const unsigned int collisionMapWidth = collisionMapBounds.getWidth();
   
   bool** passibilityMap = map->getPassibilityMatrix();
   collisionMap = new TileState*[collisionMapHeight];
   for(unsigned int y = 0; y < collisionMapHeight; ++y)
   {
      TileState* row = collisionMap[y] = new TileState[collisionMapWidth];
      for(unsigned int x = 0; x < collisionMapWidth; ++x)
      {
         bool passible = passibilityMap[y / collisionTileRatio][x / collisionTileRatio];
         row[x].entityType = passible ? TileState::FREE : TileState::OBSTACLE;
         row[x].entity = NULL;
      }
   }

   pathfinder.initialize(collisionMap, MOVEMENT_TILE_SIZE, collisionMapBounds);
   DEBUG("Entity grid initialized.");
}

const std::string& EntityGrid::getMapName() const
{
   if(!map)
   {
      T_T("Requested map name when map does not exist.");
   }
   
   return map->getName();
}

const shapes::Rectangle& EntityGrid::getMapBounds() const
{
   if(!map)
   {
      T_T("Requested map bounds when map does not exist.");
   }

   return map->getBounds();
}

bool EntityGrid::withinMap(const shapes::Point2D& point) const
{
   shapes::Rectangle pixelBounds(shapes::Point2D::ORIGIN, getMapBounds().getSize() * TileEngine::TILE_SIZE);
   return pixelBounds.contains(point);
}

void EntityGrid::step(long timePassed)
{
   if(map) map->step(timePassed);
}

EntityGrid::Path EntityGrid::findBestPath(const shapes::Point2D& src, const shapes::Point2D& dst)
{
   return pathfinder.findBestPath(src, dst);
}

EntityGrid::Path EntityGrid::findReroutedPath(const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size)
{
   return pathfinder.findReroutedPath(*this, src, dst, size);
}

bool EntityGrid::addObstacle(const shapes::Point2D& location, const shapes::Size& size)
{
   return occupyArea(shapes::Rectangle(location, size), TileState(TileState::OBSTACLE));
}

bool EntityGrid::addActor(Actor* actor, const shapes::Point2D& area)
{
   return occupyArea(shapes::Rectangle(area, actor->getSize()), TileState(TileState::ACTOR, actor));
}

bool EntityGrid::changeActorLocation(Actor* actor, const shapes::Point2D& dst)
{
   const shapes::Rectangle dstArea(dst, actor->getSize());
   TileState actorState(TileState::ACTOR, actor);
   if(occupyArea(dstArea, actorState))
   {
      freeArea(actor->getLocation(), dst, actor->getSize(), actorState);
      return true;
   }

   return false;
}

void EntityGrid::removeActor(Actor* actor)
{
   freeArea(shapes::Rectangle(actor->getLocation(), actor->getSize()));
}

Actor* EntityGrid::getAdjacentActor(Actor* actor) const
{
   if(collisionMap == NULL)
   {
      return NULL;
   }
   
   shapes::Point2D adjacentLocation = actor->getLocation();
   const shapes::Size& actorSize = actor->getSize();
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
         adjacentLocation.x += actorSize.width;
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
         adjacentLocation.y += actorSize.height;
         break;
      }
      default:
      {
         break;
      }
   }

   int rectLeft = std::max(0, adjacentLocation.x/MOVEMENT_TILE_SIZE);
   int rectRight = std::min(collisionMapBounds.getWidth(), (adjacentLocation.x + actorSize.width - 1)/MOVEMENT_TILE_SIZE);
   int rectTop = std::max(0, adjacentLocation.y/MOVEMENT_TILE_SIZE);
   int rectBottom = std::min(collisionMapBounds.getHeight(), (adjacentLocation.y + actorSize.height - 1)/MOVEMENT_TILE_SIZE);
   
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

bool EntityGrid::canOccupyArea(const shapes::Rectangle& area, TileState state) const
{
   if(collisionMap == NULL || state.entityType == TileState::FREE)
   {
      return false;
   }
   
   shapes::Rectangle areaRect = getCollisionMapEdges(area);
   
   if(!collisionMapBounds.contains(areaRect))
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

bool EntityGrid::occupyArea(const shapes::Rectangle& area, TileState state)
{
   shapes::Rectangle areaRect = getCollisionMapEdges(area);

   if(canOccupyArea(area, state))
   {
      setArea(areaRect, state);
      return true;
   }

   DEBUG("Couldn't occupy tiles from %d,%d to %d,%d", areaRect.left, areaRect.top, areaRect.right, areaRect.bottom);
   return false;
}

void EntityGrid::freeArea(const shapes::Rectangle& areaToFree)
{
   shapes::Rectangle rectToFree = getCollisionMapEdges(areaToFree);
   
   setArea(rectToFree, TileState(TileState::FREE));
}

void EntityGrid::freeArea(const shapes::Point2D& previousLocation, const shapes::Point2D& currentLocation, const shapes::Size& size, TileState state)
{
   freeArea(shapes::Rectangle(previousLocation, size));

   shapes::Rectangle currentRect = getCollisionMapEdges(shapes::Rectangle(currentLocation, size));
   setArea(currentRect, state);
}

bool EntityGrid::isAreaFree(const shapes::Rectangle& area) const
{
   if(collisionMap == NULL) return false;

   shapes::Rectangle areaRect = getCollisionMapEdges(area);

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

void EntityGrid::moveToClosestPoint(Actor* actor, int xDirection, int yDirection, int distance)
{
   if(xDirection == 0 && yDirection == 0) return;
   if(distance == 0) return;

   TileState actorState(TileState::ACTOR, actor);

   const shapes::Point2D& source = actor->getLocation();
   const shapes::Size& actorSize = actor->getSize();
   
   const shapes::Size mapPixelSize = shapes::Size(collisionMapBounds.getWidth(), collisionMapBounds.getHeight()) * MOVEMENT_TILE_SIZE;
   
   shapes::Point2D lastAvailablePoint = source;
   
   while(distance > 0)
   {
      int distanceTraversed = std::min(distance, MOVEMENT_TILE_SIZE / 2);
      distance -= distanceTraversed;
      
      // Get the next point for movement, and clamp it to the map Size
      shapes::Point2D nextPoint;
      nextPoint.x = lastAvailablePoint.x + xDirection * distanceTraversed;
      nextPoint.x = std::max(nextPoint.x, 0);
      nextPoint.x = std::min(nextPoint.x, static_cast<int>(mapPixelSize.width) - MOVEMENT_TILE_SIZE);

      nextPoint.y = lastAvailablePoint.y + yDirection * distanceTraversed;
      nextPoint.y = std::max(nextPoint.y, 0);
      nextPoint.y = std::min(nextPoint.y, static_cast<int>(mapPixelSize.height) - MOVEMENT_TILE_SIZE);

      if(lastAvailablePoint == nextPoint)
      {
         // We haven't moved any further, so we are at a dead end
         break;
      }

      if(!canOccupyArea(shapes::Rectangle(nextPoint, actorSize), actorState))
      {
         break;
      }

      lastAvailablePoint = nextPoint;      
   }

   if(lastAvailablePoint != source)
   {
      if(!occupyArea(shapes::Rectangle(lastAvailablePoint, actor->getSize()), actorState))
      {
         // If updating failed, just stick with the start location
         occupyArea(shapes::Rectangle(source, actorSize), actorState);
      }
      else
      {
         // If we moved, update the map accordingly
         freeArea(source, lastAvailablePoint, actorSize, actorState);

         actor->setLocation(lastAvailablePoint);
      }
   }
}

bool EntityGrid::beginMovement(Actor* actor, const shapes::Point2D& dst)
{
   return occupyArea(shapes::Rectangle(dst, actor->getSize()), TileState(TileState::ACTOR, actor));
}

void EntityGrid::abortMovement(Actor* actor, const shapes::Point2D& src, const shapes::Point2D& dst)
{
   freeArea(src, actor->getLocation(), actor->getSize(), TileState(TileState::ACTOR, actor));
   freeArea(dst, actor->getLocation(), actor->getSize(), TileState(TileState::ACTOR, actor));
}

void EntityGrid::endMovement(Actor* actor, const shapes::Point2D& src, const shapes::Point2D& dst)
{
   freeArea(src, dst, actor->getSize(), TileState(TileState::ACTOR, actor));
}

void EntityGrid::setArea(const shapes::Rectangle& area, TileState state)
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

void EntityGrid::drawBackground(int y) const
{
   if(map == NULL) return;

   if(DRAW_ENTITY_GRID)
   {
      for(unsigned int x = 0; x < collisionMapBounds.getWidth(); ++x)
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
            case TileState::ACTOR:
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
   else
   {
      map->drawBackground(y);
   }
}

void EntityGrid::drawForeground(int y) const
{
   map->drawForeground(y);
}

void EntityGrid::receive(const ActorMoveMessage& message)
{
   const std::vector<MapExit>& mapExits = map->getMapExits();
   for(std::vector<MapExit>::const_iterator iter = mapExits.begin(); iter != mapExits.end(); ++iter)
   {
      if(message.movingActor == tileEngine.getPlayerCharacter()
            && !iter->getBounds().contains(message.oldLocation)
            && iter->getBounds().contains(message.newLocation))
      {
         messagePipe.sendMessage(MapExitMessage(*iter));
         return;
      }
   }

   const std::vector<TriggerZone>& triggerZones = map->getTriggerZones();
   for(std::vector<TriggerZone>::const_iterator iter = triggerZones.begin(); iter != triggerZones.end(); ++iter)
   {
      if(!iter->getBounds().contains(message.oldLocation)
            && iter->getBounds().contains(message.newLocation))
      {
         messagePipe.sendMessage(MapTriggerMessage(*iter, message.movingActor));
      }
   }
}

void EntityGrid::clearMap()
{
   deleteCollisionMap();
}

void EntityGrid::deleteCollisionMap()
{
   if(collisionMap)
   {
      for(unsigned int i = 0; i < collisionMapBounds.getHeight(); ++i)
      {
         delete [] collisionMap[i];
      }
      
      delete [] collisionMap;
      collisionMap = NULL;
   }
}

EntityGrid::~EntityGrid()
{
   clearMap();
   messagePipe.unregisterListener(this);
}
