/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "GridActor.h"

#include "GridActor_Orders.h"
#include "GridActorMoveMessage.h"
#include "EntityGrid.h"
#include "MessagePipe.h"
#include "Direction.h"
#include "Sprite.h"
#include "TileEngine.h"

#define DEBUG_FLAG DEBUG_ACTOR

GridActor::GridActor(const std::string& name, messaging::MessagePipe& messagePipe, EntityGrid& entityGrid, const geometry::Point2D& location, const geometry::Size& size, double movementSpeed, geometry::Direction direction) :
   Actor(name, location, size, direction),
   m_movementSpeed(movementSpeed),
   m_entityGrid(entityGrid),
   m_messagePipe(messagePipe)
{
}

GridActor::~GridActor() = default;

void GridActor::flushOrders()
{
   while(!m_orders.empty())
   {
      m_orders.pop();
   }
}

void GridActor::step(long timePassed)
{
   Actor::step(timePassed);

   if(!isIdle())
   {
      auto& currentOrder = m_orders.front();
      if(currentOrder->perform(timePassed))
      {
         m_orders.pop();
      }
   }
}

void GridActor::draw() const
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

bool GridActor::isIdle() const
{
   return m_orders.empty();
}

void GridActor::move(const geometry::Point2D& dst, const std::shared_ptr<Task>& task)
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

void GridActor::stand(geometry::Direction direction)
{
   m_orders.emplace(new StandOrder(*this, direction));
}

void GridActor::faceActor(GridActor* other)
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

void GridActor::setLocation(const geometry::Point2D& location)
{
   const auto oldLocation = m_pixelLoc;
   Actor::setLocation(location);
   m_messagePipe.sendMessage(GridActorMoveMessage(oldLocation, location, this));
}

void GridActor::setMovementSpeed(float speed)
{
   m_movementSpeed = speed;
}

float GridActor::getMovementSpeed() const
{
   return m_movementSpeed;
}
