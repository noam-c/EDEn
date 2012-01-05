/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"
#include "NPC_Orders.h"

NPC::StandOrder::StandOrder(NPC& npc, MovementDirection direction) : Order(npc), direction(direction)
{
}

bool NPC::StandOrder::perform(long timePassed)
{
   npc.setDirection(direction);
   npc.setFrame(STANDING_PREFIX);
   return true;
}
