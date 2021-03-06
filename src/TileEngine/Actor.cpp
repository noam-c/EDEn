/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Actor.h"

#include "ActorMoveMessage.h"
#include "ActorMoveOrder.h"
#include "ActorStandOrder.h"
#include "EntityGrid.h"
#include "MessagePipe.h"
#include "Direction.h"
#include "ResourceLoader.h"
#include "Sprite.h"
#include "TileEngine.h"

const std::string Actor::DEFAULT_WALKING_PREFIX = "walk";
const std::string Actor::DEFAULT_STANDING_PREFIX = "stand";

#define DEBUG_FLAG DEBUG_ACTOR

Actor::Actor(const std::string& name, messaging::MessagePipe& messagePipe, EntityGrid& entityGrid, const geometry::Point2D& location, const geometry::Size& size, double movementSpeed, geometry::Direction direction) :
   m_name(name),
   m_pixelLoc(location),
   m_size(size),
   m_movementSpeed(movementSpeed),
   m_currDirection(direction),
   m_sprite(nullptr),
   m_entityGrid(entityGrid),
   m_messagePipe(messagePipe)
{
}

Actor::~Actor() = default;

void Actor::flushOrders()
{
   while(!m_orders.empty())
   {
      m_orders.pop();
   }
}

const std::string& Actor::getName() const
{
   return m_name;
}

const geometry::Size& Actor::getSize() const
{
   return m_size;
}

void Actor::step(long timePassed)
{
   if(m_sprite)
   {
      m_sprite->step(timePassed);
   }

   if(!isIdle())
   {
      auto& currentOrder = m_orders.front();
      if(currentOrder->perform(timePassed))
      {
         m_orders.pop();
      }
   }
}

void Actor::draw() const
{
   if(m_sprite)
   {
      m_sprite->draw({ m_pixelLoc.x, m_pixelLoc.y + TileEngine::TILE_SIZE });
   }

   if(!m_orders.empty())
   {
      m_orders.front()->draw();
   }
}

bool Actor::isIdle() const
{
   return m_orders.empty();
}

void Actor::move(const geometry::Point2D& dst, const std::shared_ptr<Task>& task)
{
   if(m_entityGrid.withinMap(dst))
   {
      DEBUG("Sending move order to %s: %d,%d", m_name.c_str(), dst.x, dst.y);
      m_orders.emplace(new MoveOrder(*this, task, dst, m_entityGrid));
   }
   else
   {
      DEBUG("%d,%d is not a place!!!", dst.x, dst.y);
   }
}

void Actor::stand(geometry::Direction direction)
{
   m_orders.emplace(new StandOrder(*this, direction));
}

void Actor::faceActor(Actor* other)
{
   geometry::Point2D currentLocation = getLocation();
   geometry::Point2D otherLocation = other->getLocation();
   geometry::Direction directionToOther = getDirection();

   int xDiff = currentLocation.x - otherLocation.x;
   int yDiff = currentLocation.y - otherLocation.y;

   if(abs(xDiff) > abs(yDiff))
   {
      if(xDiff < 0)
      {
         directionToOther = geometry::Direction::RIGHT;
      }
      else if(xDiff > 0)
      {
         directionToOther = geometry::Direction::LEFT;
      }
   }
   else
   {
      if(yDiff < 0)
      {
         directionToOther = geometry::Direction::DOWN;
      }
      else if(yDiff > 0)
      {
         directionToOther = geometry::Direction::UP;
      }
   }

   stand(directionToOther);
}

void Actor::setSpritesheet(const std::string& sheetName)
{
   std::shared_ptr<Spritesheet> sheet = ResourceLoader::getSpritesheet(sheetName);
   if(!m_sprite)
   {
      m_sprite.reset(new Sprite(sheet));
   }
   else
   {
      m_sprite->setSheet(sheet);
   }

   m_sprite->setFrame(Actor::DEFAULT_STANDING_PREFIX, m_currDirection);
}

void Actor::setFrame(const std::string& frameName)
{
   if(!m_sprite)
   {
      DEBUG("Failed to set sprite frame because actor %s doesn't have a sprite.", m_name.c_str());
      return;
   }

   m_sprite->setFrame(frameName, m_currDirection);
}

void Actor::setAnimation(const std::string& animationName)
{
   if(!m_sprite)
   {
      DEBUG("Failed to set sprite animation because actor %s doesn't have a sprite.", m_name.c_str());
      return;
   }

   m_sprite->setAnimation(animationName, m_currDirection);
}

void Actor::setLocation(const geometry::Point2D& location)
{
   const geometry::Point2D oldLocation = m_pixelLoc;
   m_pixelLoc = location;
   m_messagePipe.sendMessage(ActorMoveMessage(oldLocation, location, this));
}

const geometry::Point2D& Actor::getLocation() const
{
   return m_pixelLoc;
}

void Actor::setDirection(geometry::Direction direction)
{
   m_currDirection = direction;
}

geometry::Direction Actor::getDirection() const
{
   return m_currDirection;
}

void Actor::setMovementSpeed(float speed)
{
   m_movementSpeed = speed;
}

float Actor::getMovementSpeed() const
{
   return m_movementSpeed;
}
