/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "GridActorStandOrder.h"

GridActor::StandOrder::StandOrder(GridActor& gridActor, geometry::Direction direction) :
   Order(gridActor),
   m_direction(direction)
{
}

bool GridActor::StandOrder::perform(long timePassed)
{
   m_gridActor.setDirection(m_direction);
   m_gridActor.setFrame(Actor::DEFAULT_STANDING_PREFIX);
   return true;
}
