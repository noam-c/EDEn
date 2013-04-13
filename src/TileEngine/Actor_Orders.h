/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_ORDER_H
#define ACTOR_ORDER_H

#include "EntityGrid.h"

/**
 * An abstract class for asynchronous Actor instructions.
 * These instructions are meant to be sent to an Actor and then
 * processed using a call to the Order's <code>perform</code> function
 * in each frame.
 *
 * @author Noam Chitayat
 */
class Actor::Order
{
   protected:
      Actor& actor;
      Order(Actor& actor) : actor(actor) {}
   
   public:
      virtual bool perform(long timePassed) = 0;
      virtual void draw() {}
      virtual ~Order() {}
};

/**
 * An order that causes the Actor to stand still, facing a
 * specified direction.
 *
 * @author Noam Chitayat
 */
class Actor::StandOrder : public Actor::Order
{
   MovementDirection direction;

   public:
      StandOrder(Actor& actor, MovementDirection direction);
      bool perform(long timePassed);
};

/**
 * An order that causes the Actor to move to a specified
 * destination point on the map.
 *
 * @author Noam Chitayat
 */
class Actor::MoveOrder : public Actor::Order
{
   /**
    * Tracks if the move order has calculated a path from
    * the Actor's current location to the destination.
    */
   bool pathInitialized;

   /**
    * Tracks if the Actor has begun movement
    * towards the next node in its path.
    */
   bool movementBegun;

   /** The destination that the Actor will move to. */
   const shapes::Point2D dst;

   /** The last node that the Actor successfully moved to. */
   shapes::Point2D lastWaypoint;

   /** The next node that the Actor will move to. */
   shapes::Point2D nextWaypoint;

   /** The grid that the Actor is moving along. */
   EntityGrid& entityGrid;

   /** The path that the Actor will use to get to the destination. */
   EntityGrid::Path path;

   /** Total distance for the character to move. */
   float cumulativeDistanceCovered;

   /**
    * Update the direction that the Actor is facing, as well as the sprite used.
    *
    * @param newDirection The new direction to face.
    * @param moving True iff the Actor is moving toward another destination.
    */
   void updateDirection(MovementDirection newDirection, bool moving);
   void updateNextWaypoint(shapes::Point2D location, MovementDirection& direction);

   public:
      /**
       * Constructor.
       *
       * @param actor The Actor that will be following this Move order.
       * @param destination The point that the Actor will move to.
       * @param entityGrid The grid that the Actor is moving on.
       */
      MoveOrder(Actor& actor, const shapes::Point2D& destination, EntityGrid& entityGrid);

      /**
       * Destructor.
       */
      ~MoveOrder();
      bool perform(long timePassed);
      void draw();
};

#endif
