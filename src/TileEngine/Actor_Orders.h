/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_ORDER_H
#define ACTOR_ORDER_H

#include "EntityGrid.h"

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

class Actor::StandOrder : public Actor::Order
{
   MovementDirection direction;

   public:
      StandOrder(Actor& actor, MovementDirection direction);
      bool perform(long timePassed);
};

class Actor::MoveOrder : public Actor::Order
{
   bool pathInitialized;
   bool movementBegun;
   const shapes::Point2D dst;
   shapes::Point2D lastWaypoint;
   shapes::Point2D nextWaypoint;
   EntityGrid& entityGrid;
   EntityGrid::Path path;

   /** Total distance for the character to move. */
   float cumulativeDistanceCovered;

   void updateDirection(MovementDirection newDirection, bool moving);
   void updateNextWaypoint(shapes::Point2D location, MovementDirection& direction);

   public:
      MoveOrder(Actor& actor, const shapes::Point2D& destination, EntityGrid& entityGrid);
      ~MoveOrder();
      bool perform(long timePassed);
      void draw();
};

const std::string WALKING_PREFIX = "walk";
const std::string STANDING_PREFIX = "stand";

#endif
