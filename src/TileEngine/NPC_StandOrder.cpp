/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"
#include "NPC_Orders.h"

NPC::StandOrder::StandOrder(NPC& npc) : Order(npc)
{
}

bool NPC::StandOrder::perform(long timePassed)
{
   npc.setFrame(STANDING_PREFIX);
   return true;
}