/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Actor.h"
#include "Actor_Orders.h"

Actor::StandOrder::StandOrder(Actor& actor, geometry::Direction direction) :
   Order(actor),
   m_direction(direction)
{
}

bool Actor::StandOrder::perform(long timePassed)
{
   m_actor.setDirection(m_direction);
   m_actor.setFrame(Actor::DEFAULT_STANDING_PREFIX);
   return true;
}
