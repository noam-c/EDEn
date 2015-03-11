/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "EntityGrid.h"

#include "SDL_opengl.h"

#include "Actor.h"
#include "ActorMoveMessage.h"
#include "Map.h"
#include "MapExit.h"
#include "MapExitMessage.h"
#include "MapTriggerMessage.h"
#include "MessagePipe.h"
#include "MovementDirection.h"
#include "PlayerCharacter.h"
#include "Point2D.h"
#include "Rectangle.h"
#include "TileEngine.h"
#include "TileState.h"
#include "TriggerZone.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ENTITY_GRID

// Define as 1 to draw the entity grid's state instead of the map
#define DRAW_ENTITY_GRID 0

// Movement tile size can be set to a divisor of drawn tile size to increase the pathfinding graph size
// For now, no need for the additional granularity
const int EntityGrid::MOVEMENT_TILE_SIZE = 32;

const float EntityGrid::ROOT_2 = 1.41421356f;
const float EntityGrid::INFINITY = std::numeric_limits<float>::infinity();

EntityGrid::EntityGrid(const TileEngine& tileEngine, messaging::MessagePipe& messagePipe) :
   m_tileEngine(tileEngine),
   m_messagePipe(messagePipe)
{
   messagePipe.registerListener(this);
}

EntityGrid::~EntityGrid()
{
   m_messagePipe.unregisterListener(this);
}

geometry::Rectangle EntityGrid::getCollisionMapEdges(const geometry::Rectangle& area) const
{
   const geometry::Point2D collisionMapTopLeft(area.left, area.top);
   const geometry::Point2D collisionMapBottomRight(area.right - 1, area.bottom - 1);

   return geometry::Rectangle(collisionMapTopLeft / MOVEMENT_TILE_SIZE, collisionMapBottomRight / MOVEMENT_TILE_SIZE);
}

const geometry::Point2D& EntityGrid::getMapEntrance(const std::string& exitedMapName) const
{
   std::shared_ptr<const Map> map(m_map.lock());
   if(!map)
   {
      T_T("Requested map entry point when map does not exist.");
   }

   return map->getMapEntrance(exitedMapName);
}

bool EntityGrid::hasMapData() const
{
   return !m_map.expired();
}

void EntityGrid::setMapData(std::weak_ptr<const Map> mapData)
{
   DEBUG("Resetting entity grid...");
   m_collisionMap.clear();
   m_map = mapData;

   std::shared_ptr<const Map> map(m_map.lock());
   if(!map) return;

   const int collisionTileRatio = TileEngine::TILE_SIZE / MOVEMENT_TILE_SIZE;

   auto collisionMapSize = map->getBounds().getSize() * collisionTileRatio;
   m_collisionMapBounds = geometry::Rectangle(geometry::Point2D::ORIGIN, collisionMapSize);

   const unsigned int collisionMapHeight = collisionMapSize.height;
   const unsigned int collisionMapWidth = collisionMapSize.width;

   m_collisionMap.resize(collisionMapSize);
   for(unsigned int x = 0; x < collisionMapWidth; ++x)
   {
      for(unsigned int y = 0; y < collisionMapHeight; ++y)
      {
         bool passible = map->isPassible(x / collisionTileRatio, y / collisionTileRatio);
         m_collisionMap(x, y).entityType = passible ? TileState::EntityType::FREE : TileState::EntityType::OBSTACLE;
      }
   }

   m_pathfinder.initialize(m_collisionMap, MOVEMENT_TILE_SIZE, m_collisionMapBounds);
   DEBUG("Entity grid initialized.");
}

const std::string& EntityGrid::getMapName() const
{
   std::shared_ptr<const Map> map(m_map.lock());
   if(!map)
   {
      T_T("Requested map name when map does not exist.");
   }

   return map->getName();
}

const geometry::Rectangle& EntityGrid::getMapBounds() const
{
   std::shared_ptr<const Map> map(m_map.lock());
   if(!map)
   {
      T_T("Requested map bounds when map does not exist.");
   }

   return map->getBounds();
}

bool EntityGrid::withinMap(const geometry::Point2D& point) const
{
   geometry::Rectangle pixelBounds(geometry::Point2D::ORIGIN, getMapBounds().getSize() * TileEngine::TILE_SIZE);
   return pixelBounds.contains(point);
}

void EntityGrid::step(long timePassed)
{
   std::shared_ptr<const Map> map(m_map.lock());
   if(map)
   {
      map->step(timePassed);
   }
}

EntityGrid::Path EntityGrid::findBestPath(const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size)
{
   return m_pathfinder.findBestPath(*this, src, dst, size);
}

EntityGrid::Path EntityGrid::findReroutedPath(const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size)
{
   return m_pathfinder.findReroutedPath(*this, src, dst, size);
}

bool EntityGrid::addObstacle(const geometry::Point2D& location, const geometry::Size& size)
{
   return occupyArea(geometry::Rectangle(location, size), TileState(TileState::EntityType::OBSTACLE));
}

bool EntityGrid::addActor(Actor* actor, const geometry::Point2D& area)
{
   return occupyArea(geometry::Rectangle(area, actor->getSize()), TileState(TileState::EntityType::ACTOR, actor));
}

bool EntityGrid::changeActorLocation(Actor* actor, const geometry::Point2D& dst)
{
   const geometry::Rectangle dstArea(dst, actor->getSize());
   TileState actorState(TileState::EntityType::ACTOR, actor);
   if(occupyArea(dstArea, actorState))
   {
      freeArea(actor->getLocation(), dst, actor->getSize(), actorState);
      return true;
   }

   return false;
}

void EntityGrid::removeActor(Actor* actor)
{
   freeArea(geometry::Rectangle(actor->getLocation(), actor->getSize()));
}

Actor* EntityGrid::getAdjacentActor(Actor* actor) const
{
   if(m_collisionMap.empty())
   {
      return nullptr;
   }

   geometry::Point2D adjacentLocation = actor->getLocation();
   const geometry::Size& actorSize = actor->getSize();
   const MovementDirection direction = actor->getDirection();
   switch(direction)
   {
      case MovementDirection::UP_LEFT:
      case MovementDirection::LEFT:
      case MovementDirection::DOWN_LEFT:
      {
         adjacentLocation.x -= MOVEMENT_TILE_SIZE;
         break;
      }
      case MovementDirection::UP_RIGHT:
      case MovementDirection::RIGHT:
      case MovementDirection::DOWN_RIGHT:
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
      case MovementDirection::UP_RIGHT:
      case MovementDirection::UP:
      case MovementDirection::UP_LEFT:
      {
         adjacentLocation.y -= MOVEMENT_TILE_SIZE;
         break;
      }
      case MovementDirection::DOWN_LEFT:
      case MovementDirection::DOWN:
      case MovementDirection::DOWN_RIGHT:
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
   int rectRight = std::min(m_collisionMapBounds.getWidth(), (adjacentLocation.x + actorSize.width - 1)/MOVEMENT_TILE_SIZE);
   int rectTop = std::max(0, adjacentLocation.y/MOVEMENT_TILE_SIZE);
   int rectBottom = std::min(m_collisionMapBounds.getHeight(), (adjacentLocation.y + actorSize.height - 1)/MOVEMENT_TILE_SIZE);

   for(int rectY = rectTop; rectY <= rectBottom; ++rectY)
   {
      for(int rectX = rectLeft; rectX <= rectRight; ++rectX)
      {
         const TileState& collisionTile = m_collisionMap(rectX, rectY);
         if(collisionTile.entityType == TileState::EntityType::ACTOR && collisionTile.entity != actor)
         {
            return static_cast<Actor*>(collisionTile.entity);
         }
      }
   }

   return nullptr;
}

bool EntityGrid::canOccupyArea(const geometry::Rectangle& area, TileState state) const
{
   if(m_collisionMap.empty() || state.entityType == TileState::EntityType::FREE)
   {
      return false;
   }

   geometry::Rectangle areaRect = getCollisionMapEdges(area);

   if(!m_collisionMapBounds.contains(areaRect))
   {
      return false;
   }

   for(int collisionMapY = areaRect.top; collisionMapY <= areaRect.bottom; ++collisionMapY)
   {
      for(int collisionMapX = areaRect.left; collisionMapX <= areaRect.right; ++collisionMapX)
      {
         // We cannot occupy the point if it is reserved by an entity other than the entity attempting to occupy it.
         // For instance, we cannot occupy a tile already occupied by an obstacle or a different character.
         const TileState& collisionTile = m_collisionMap(collisionMapX, collisionMapY);
         if(collisionTile.entityType != TileState::EntityType::FREE)
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

bool EntityGrid::occupyArea(const geometry::Rectangle& area, TileState state)
{
   geometry::Rectangle areaRect = getCollisionMapEdges(area);

   if(canOccupyArea(area, state))
   {
      setArea(areaRect, state);
      return true;
   }

   DEBUG("Couldn't occupy tiles from %d,%d to %d,%d", areaRect.left, areaRect.top, areaRect.right, areaRect.bottom);
   return false;
}

void EntityGrid::freeArea(const geometry::Rectangle& areaToFree)
{
   geometry::Rectangle rectToFree = getCollisionMapEdges(areaToFree);

   setArea(rectToFree, TileState(TileState::EntityType::FREE));
}

void EntityGrid::freeArea(const geometry::Point2D& previousLocation, const geometry::Point2D& currentLocation, const geometry::Size& size, TileState state)
{
   freeArea(geometry::Rectangle(previousLocation, size));

   geometry::Rectangle currentRect = getCollisionMapEdges(geometry::Rectangle(currentLocation, size));
   setArea(currentRect, state);
}

bool EntityGrid::isAreaFree(const geometry::Rectangle& area) const
{
   if(m_collisionMap.empty()) return false;

   geometry::Rectangle areaRect = getCollisionMapEdges(area);

   for(int collisionMapY = areaRect.top; collisionMapY < areaRect.bottom; ++collisionMapY)
   {
      for(int collisionMapX = areaRect.left; collisionMapX < areaRect.right; ++collisionMapX)
      {
         // We cannot occupy the point if it is reserved by an obstacle or a character.
         const TileState& collisionTile = m_collisionMap(collisionMapX, collisionMapY);
         if(collisionTile.entityType != TileState::EntityType::FREE)
         {
            return false;
         }
      }
   }

   return true;
}

bool EntityGrid::moveToClosestPoint(Actor* actor, int xDirection, int yDirection, int distance)
{
   if(xDirection == 0 && yDirection == 0) return false;
   if(distance == 0) return false;

   TileState actorState(TileState::EntityType::ACTOR, actor);

   const geometry::Point2D& source = actor->getLocation();
   const geometry::Size& actorSize = actor->getSize();

   const geometry::Size mapPixelSize = geometry::Size(m_collisionMapBounds.getWidth(), m_collisionMapBounds.getHeight()) * MOVEMENT_TILE_SIZE;

   geometry::Point2D lastAvailablePoint = source;

   while(distance > 0)
   {
      int distanceTraversed = std::min(distance, MOVEMENT_TILE_SIZE / 2);
      distance -= distanceTraversed;

      // Get the next point for movement, and clamp it to the map Size
      geometry::Point2D nextPoint;
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

      if(!canOccupyArea(geometry::Rectangle(nextPoint, actorSize), actorState))
      {
         geometry::Point2D nextHorizontalPoint(nextPoint.x, lastAvailablePoint.y);
         geometry::Point2D nextVerticalPoint(lastAvailablePoint.x, nextPoint.y);

         if(canOccupyArea(geometry::Rectangle(nextHorizontalPoint, actorSize), actorState))
         {
            lastAvailablePoint = nextHorizontalPoint;
         }
         else if(canOccupyArea(geometry::Rectangle(nextVerticalPoint, actorSize), actorState))
         {
            lastAvailablePoint = nextVerticalPoint;
         }

         break;
      }

      lastAvailablePoint = nextPoint;
   }

   if(lastAvailablePoint != source)
   {
      if(occupyArea(geometry::Rectangle(lastAvailablePoint, actor->getSize()), actorState))
      {
         // If we moved, update the map accordingly
         freeArea(source, lastAvailablePoint, actorSize, actorState);

         actor->setLocation(lastAvailablePoint);
         return true;
      }
      else
      {
         // If updating failed, just stick with the start location
         occupyArea(geometry::Rectangle(source, actorSize), actorState);
      }
   }

   return false;
}

bool EntityGrid::beginMovement(Actor* actor, const geometry::Point2D& dst)
{
   return occupyArea(geometry::Rectangle(dst, actor->getSize()), TileState(TileState::EntityType::ACTOR, actor));
}

void EntityGrid::abortMovement(Actor* actor, const geometry::Point2D& src, const geometry::Point2D& dst)
{
   freeArea(src, actor->getLocation(), actor->getSize(), TileState(TileState::EntityType::ACTOR, actor));
   freeArea(dst, actor->getLocation(), actor->getSize(), TileState(TileState::EntityType::ACTOR, actor));
}

void EntityGrid::endMovement(Actor* actor, const geometry::Point2D& src, const geometry::Point2D& dst)
{
   freeArea(src, dst, actor->getSize(), TileState(TileState::EntityType::ACTOR, actor));
}

void EntityGrid::setArea(const geometry::Rectangle& area, TileState state)
{
   if(m_collisionMap.empty()) return;

   for(int collisionMapY = area.top; collisionMapY <= area.bottom; ++collisionMapY)
   {
      for(int collisionMapX = area.left; collisionMapX <= area.right; ++collisionMapX)
      {
         m_collisionMap(collisionMapX, collisionMapY) = state;
      }
   }
}

void EntityGrid::drawBackground(int y) const
{
   std::shared_ptr<const Map> map(m_map.lock());
   if(!map)
   {
      return;
   }

   if(DRAW_ENTITY_GRID)
   {
      for(unsigned int x = 0; x < m_collisionMapBounds.getWidth(); ++x)
      {
         float destLeft = float(x * MOVEMENT_TILE_SIZE);
         float destRight = float((x + 1) * MOVEMENT_TILE_SIZE);
         float destTop = float(y * MOVEMENT_TILE_SIZE);
         float destBottom = float((y + 1) * MOVEMENT_TILE_SIZE);

         glDisable(GL_TEXTURE_2D);
         glBegin(GL_QUADS);

         switch(m_collisionMap(x, y).entityType)
         {
            case TileState::EntityType::FREE:
            {
               glColor3f(0.0f, 0.5f, 0.0f);
               break;
            }
            case TileState::EntityType::ACTOR:
            {
               if(m_collisionMap(x, y).entity == nullptr)
               {
                  glColor3f(0.5f, 0.0f, 0.0f);
               }
               else
               {
                  glColor3f(0.0f, 0.0f, 0.5f);
               }
               break;
            }
            case TileState::EntityType::OBSTACLE:
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
   std::shared_ptr<const Map> map(m_map.lock());
   if(map)
   {
      map->drawForeground(y);
   }
}

void EntityGrid::receive(const ActorMoveMessage& message)
{
   std::shared_ptr<const Map> map(m_map.lock());
   if(!map)
   {
      return;
   }

   const std::vector<MapExit>& mapExits = map->getMapExits();
   for(const auto& mapExit : mapExits)
   {
      if(m_tileEngine.isPlayerCharacter(message.movingActor) &&
            !mapExit.getBounds().contains(message.oldLocation) &&
            mapExit.getBounds().contains(message.newLocation))
      {
         m_messagePipe.sendMessage(MapExitMessage(mapExit));
         return;
      }
   }

   const std::vector<TriggerZone>& triggerZones = map->getTriggerZones();
   for(const auto& triggerZone : triggerZones)
   {
      if(!triggerZone.getBounds().contains(message.oldLocation)
            && triggerZone.getBounds().contains(message.newLocation))
      {
         m_messagePipe.sendMessage(MapTriggerMessage(triggerZone, message.movingActor));
      }
   }
}

