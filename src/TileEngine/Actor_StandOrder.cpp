/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Actor.h"
#include "Actor_Orders.h"

Actor::StandOrder::StandOrder(Actor& actor, MovementDirection direction) :
   Order(actor),
   direction(direction)
{
}

bool Actor::StandOrder::perform(long timePassed)
{
   actor.setDirection(direction);
   actor.setFrame(Actor::DEFAULT_STANDING_PREFIX);
   return true;
}
