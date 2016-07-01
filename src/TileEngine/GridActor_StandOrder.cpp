/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "GridActor.h"
#include "GridActor_Orders.h"

GridActor::StandOrder::StandOrder(GridActor& actor, geometry::Direction direction) :
   Order(actor),
   m_direction(direction)
{
}

bool GridActor::StandOrder::perform(long timePassed)
{
   m_actor.setDirection(m_direction);
   m_actor.setFrame(GridActor::DEFAULT_STANDING_PREFIX);
   return true;
}
