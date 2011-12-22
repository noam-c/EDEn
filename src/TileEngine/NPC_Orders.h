/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
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
   public:
      bool perform(long timePassed);
};

class NPC::MoveOrder : public NPC::Order
{
   const Point2D dst;
   Point2D currentLocation;
   Pathfinder& pathfinder;
   Pathfinder::Path path;
   
   public:
      MoveOrder(NPC& npc, const Point2D& destination, const Map& map);
      bool perform(long timePassed);
      void draw();
};

const std::string WALKING_PREFIX = "walk_";
const std::string STANDING_PREFIX = "stand_";

#endif