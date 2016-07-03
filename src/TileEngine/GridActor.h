/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef GRID_ACTOR_H
#define GRID_ACTOR_H

#include <queue>
#include <string>
#include <memory>

#include "Actor.h"
#include "Point2D.h"
#include "Size.h"

class EntityGrid;
class Task;

namespace messaging
{
   class MessagePipe;
};

class GridActor : public Actor
{
   class Order;
   class MoveOrder;
   class StandOrder;

   /** A queue of orders for the actor to perform */
   std::queue<std::unique_ptr<Order>> m_orders;

   /** The movement speed of the actor */
   float m_movementSpeed;

   protected:
      /** The grid of actors which this Actor interacts with. */
      EntityGrid& m_entityGrid;

      /** The message pipe used to send and receive events. */
      messaging::MessagePipe& m_messagePipe;

      /**
       * Constructor for the actor.
       * Initializes the actor's coroutine and loads the associated script.
       *
       * @param name The name of the actor (must also be the name of its script).
       * @param messagePipe The message pipe used to send and receive events.
       * @param entityGrid The map that this actor will be interacting in.
       * @param location The location (in pixels) where the actor will start off.
       * @param size The size (in pixels) of the actor.
       * @param movementSpeed The speed of the actor's movement.
       * @param direction The starting direction of the actor.
       */
       GridActor(const std::string& name, messaging::MessagePipe& messagePipe, EntityGrid& entityGrid, const geometry::Point2D& location, const geometry::Size& size, double movementSpeed, geometry::Direction direction);

      /**
       * Destructor.
       */
      virtual ~GridActor() = 0;

      /**
       * Clear the Actor's current set of orders.
       */
      void flushOrders();

   public:
      /**
       * Performs a logic step of this actor, updating the current executing order.
       *
       * @param timePassed The amount of time that has passed since the last frame.
       */
      virtual void step(long timePassed);

      /**
       * This function draws the actor in its current location with its current
       * sprite animation frame.
       */
      virtual void draw() const;

      /**
       * @return true iff the NPC is not chewing on any instructions
       *              (i.e. it is doing absolutely nothing)
       */
      bool isIdle() const;

      /**
       * This function enqueues an instruction to stand facing a specified direction.
       *
       * @param direction The direction for the actor to face when standing.
       */
      void stand(geometry::Direction direction);

      /**
       * Change the actor's direction so that it is facing the specified actor.
       *
       * @param other The actor to turn and face.
       */
      void faceActor(GridActor* other);

      /**
       * Change the location of the actor.
       * NOTE: This method is used for instantly changing the
       * location of the actor. To have the actor move to a new location,
       * please invoke Actor::move instead.
       *
       * @param location The new location of the actor.
       */
      virtual void setLocation(const geometry::Point2D& location);

      /**
       * This function enqueues a movement instruction.
       *
       * @param dst The coordinates (in pixels) for the actor to move to
       */
      virtual void move(const geometry::Point2D& dst, const std::shared_ptr<Task>& task);

      /**
       * This function changes the movement speed of the actor.
       *
       * @param speed The new movement speed of the actor.
       */
      void setMovementSpeed(float speed);

      /**
       * @return The current movement speed of the actor.
       */
      float getMovementSpeed() const;
};

#endif
