/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_MOVE_MESSAGE_H
#define ACTOR_MOVE_MESSAGE_H

#include "Point2D.h"

class Actor;

struct ActorMoveMessage
{
   const shapes::Point2D oldLocation;
   const shapes::Point2D newLocation;
   const Actor* const movingActor;

   ActorMoveMessage(const shapes::Point2D& oldLocation, const shapes::Point2D& newLocation, const Actor* const movingActor);
};

#endif
