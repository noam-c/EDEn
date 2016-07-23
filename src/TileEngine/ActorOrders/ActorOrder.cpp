/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ActorOrder.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ACTOR

Actor::Order::Order(Actor& actor) :
   m_actor(actor)
{
}

Actor::Order::~Order() = default;

void Actor::Order::draw() const
{
}
