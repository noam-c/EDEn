/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_STAND_ORDER_H
#define ACTOR_STAND_ORDER_H

#include "ActorOrder.h"

namespace geometry
{
  enum class Direction;
};

/**
 * An order that causes the Actor to stand still, facing a
 * specified direction.
 *
 * @author Noam Chitayat
 */
class Actor::StandOrder final : public Actor::Order
{
   geometry::Direction m_direction;

   public:
      StandOrder(Actor& actor, geometry::Direction direction);
      bool perform(long timePassed) override;
};

#endif
