/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "GridActorMoveMessage.h"

GridActorMoveMessage::GridActorMoveMessage(const geometry::Point2D& oldLocation, const geometry::Point2D& newLocation, const GridActor* const movingActor) :
   oldLocation(oldLocation),
   newLocation(newLocation),
   movingActor(movingActor)
{}
