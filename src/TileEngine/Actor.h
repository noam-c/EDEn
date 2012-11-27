/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_H
#define ACTOR_H

#include <queue>
#include <string>

#include "MovementDirection.h"
#include "Size.h"
#include "Point2D.h"

class EntityGrid;
class Sprite;
class Spritesheet;

namespace messaging
{
   class MessagePipe;
}

class Actor
{
   /**
    * A class for asynchronous Actor instructions.
    */
   class Order;
   class MoveOrder;
   class StandOrder;

   /** The Actor's name */
   const std::string name;
   
   /** A queue of orders for the actor to perform */
   std::queue<Order*> orders;

   /** The current location of the actor (in pixels) */
   shapes::Point2D pixelLoc;
   
   /** The size of the actor (in pixels) */
   shapes::Size size;
   
   /** The movement speed of the actor */
   float movementSpeed;
   
   /** The direction that the actor is currently facing */
   MovementDirection currDirection;
   
   protected:
      /** The Actor's associated sprite, which is drawn on screen. */
      Sprite* sprite;
      
      /** The grid of actors which this Actor interacts with. */
      EntityGrid& entityGrid;
      
      /** The message pipe used to send and receive events. */
      messaging::MessagePipe& messagePipe;

      /**
       * Constructor for the actor.
       * Initializes the actor's coroutine and loads the associated script.
       * Loads a sprite for the actor based on a given Spritesheet.
       * 
       * @param name The name of the actor (must also be the name of its script).
       * @param sheetName The name of the spritesheet to use for rendering the actor.
       * @param messagePipe The message pipe used to send and receive events.
       * @param entityGrid The map that this actor will be interacting in.
       * @param location The location (in pixels) where the actor will start off.
       * @param size The size (in pixels) of the actor.
       * @param movementSpeed The speed of the actor's movement.
       * @param direction The starting direction of the actor.
       */
      Actor(const std::string& name, const std::string& sheetName, messaging::MessagePipe& messagePipe, EntityGrid& entityGrid, const shapes::Point2D& location, const shapes::Size& size, double movementSpeed, MovementDirection direction);

      /**
       * Destructor.
       */
      virtual ~Actor() = 0;

      /**
       * Clears the queue of actor orders.
       */
      void flushOrders();

   public:
      /**
       * @return The name of this Actor.
       */
      const std::string& getName() const;

      /**
       * @return The size of this Actor (in pixels).
       */
      const shapes::Size& getSize() const;

      /**
       * Performs a logic step of this NPC. During the step, the NPC works on
       * enqueued Instructions if there are any.
       *
       * @param timePassed The amount of time that has passed since the last frame.
       */
      virtual void step(long timePassed);
      
      /**
       * This function draws the actor in its current location with its current
       * sprite animation frame.
       */
      virtual void draw();

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
      void stand(MovementDirection direction);

      /**
       * Change the actor's direction so that it is facing the specified actor.
       *
       * @param other The actor to turn and face.
       */
      void faceActor(Actor* other);
   
      /**
       * This function enqueues a movement instruction.
       *
       * @param dst The coordinates (in pixels) for the actor to move to
       */
      void move(const shapes::Point2D& dst);
      
      /**
       * This function changes the actor's spritesheet.
       *
       * @param sheetName The name of the spritesheet to get.
       */
      void setSpritesheet(const std::string& sheetName);
      
      /**
       * This function changes the actor's frame.
       *
       * @param frameName The name of the frame to use.
       */
      void setFrame(const std::string& frameName);
      
      /**
       * This function changes the actor's animation.
       *
       * @param animationName The name of the animation to use.
       *
       */
      void setAnimation(const std::string& animationName);
      
      /**
       * Change the location of the actor.
       * NOTE: This method is used for instantly changing the
       * location of the actor. To have the actor move to a new location,
       * please invoke Actor::move instead.
       *
       * @param location The new location of the actor.
       */
      void setLocation(const shapes::Point2D& location);
      
      /**
       * @return The location of the actor.
       */
      const shapes::Point2D& getLocation() const;
      
      /**
       * This function changes the direction that the actor is facing.
       *
       * @param direction The new direction for the actor to face.
       */
      void setDirection(MovementDirection direction);
      
      /**
       * @return The direction that the actor is currently facing.
       */
      MovementDirection getDirection() const;
      
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
