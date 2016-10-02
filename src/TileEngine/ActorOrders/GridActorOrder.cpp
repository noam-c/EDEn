/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "GridActorOrder.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ACTOR

GridActor::Order::Order(GridActor& gridActor) :
   m_gridActor(gridActor)
{
}

GridActor::Order::~Order() = default;

void GridActor::Order::draw() const
{
}
