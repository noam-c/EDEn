/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_MOVE_MESSAGE_H
#define ACTOR_MOVE_MESSAGE_H

#include "Point2D.h"

class Actor;

struct ActorMoveMessage final
{
   const geometry::Point2D oldLocation;
   const geometry::Point2D newLocation;
   const Actor* const movingActor;

   ActorMoveMessage(const geometry::Point2D& oldLocation, const geometry::Point2D& newLocation, const Actor* const movingActor);
};

#endif
