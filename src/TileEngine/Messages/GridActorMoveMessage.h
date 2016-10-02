/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef GRID_ACTOR_MOVE_MESSAGE_H
#define GRID_ACTOR_MOVE_MESSAGE_H

#include "Point2D.h"

class GridActor;

struct GridActorMoveMessage final
{
   const geometry::Point2D oldLocation;
   const geometry::Point2D newLocation;
   const GridActor* const movingActor;

   GridActorMoveMessage(const geometry::Point2D& oldLocation, const geometry::Point2D& newLocation, const GridActor* const movingActor);
};

#endif
