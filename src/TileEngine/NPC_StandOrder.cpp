/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"
#include "NPC_Orders.h"

bool NPC::StandOrder::perform(long timePassed)
{
   switch(npc.getDirection())
   {
      case LEFT:
      {
         npc.setFrame(STANDING_PREFIX + "left");
         break;
      }
      case RIGHT:
      {
         npc.setFrame(STANDING_PREFIX + "right");
         break;
      }
      case UP:
      {
         npc.setFrame(STANDING_PREFIX + "up");
         break;
      }
      case DOWN:
      {
         npc.setFrame(STANDING_PREFIX + "down");
         break;
      }
      default:
      {
         break;
      }
   }
   
   return true;
}