/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef GRID_ACTOR_STAND_ORDER_H
#define GRID_ACTOR_STAND_ORDER_H

#include "GridActorOrder.h"

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
class GridActor::StandOrder final : public GridActor::Order
{
   geometry::Direction m_direction;

   public:
      StandOrder(GridActor& gridActor, geometry::Direction direction);
      bool perform(long timePassed) override;
};

#endif
