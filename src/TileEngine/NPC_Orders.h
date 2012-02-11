/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef NPC_ORDER_H
#define NPC_ORDER_H

#include "Pathfinder.h"

class NPC::Order
{
   protected:
      NPC& npc;
      Order(NPC& npc) : npc(npc) {}
   
   public:
      virtual bool perform(long timePassed) = 0;
      virtual void draw() {}
      virtual ~Order() {}
};

class NPC::StandOrder : public NPC::Order
{
   MovementDirection direction;

   public:
      StandOrder(NPC& npc, MovementDirection direction);
      bool perform(long timePassed);
};

class NPC::MoveOrder : public NPC::Order
{
   bool pathInitialized;
   bool movementBegun;
   const Point2D dst;
   Point2D lastWaypoint;
   Point2D nextWaypoint;
   Pathfinder& pathfinder;
   Pathfinder::Path path;

   void updateDirection(MovementDirection newDirection, bool moving);
   void updateNextWaypoint(Point2D location, MovementDirection& direction);

   public:
      MoveOrder(NPC& npc, const Point2D& destination, Pathfinder& pathfinder);
      ~MoveOrder();
      bool perform(long timePassed);
      void draw();
};

const std::string WALKING_PREFIX = "walk";
const std::string STANDING_PREFIX = "stand";

#endif
