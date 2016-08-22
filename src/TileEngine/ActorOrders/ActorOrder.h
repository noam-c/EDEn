/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ACTOR_ORDER_H
#define ACTOR_ORDER_H

#include "Actor.h"

/**
 * An abstract class for asynchronous Actor instructions.
 * These instructions are meant to be sent to an Actor and then
 * processed using a call to the Order's <code>perform</code> function
 * in each frame.
 *
 * @author Noam Chitayat
 */
class Actor::Order
{
   protected:
      Actor& m_actor;

   public:
      Order(Actor& actor);
      virtual ~Order() = 0;
      virtual bool perform(long timePassed) = 0;
      virtual void draw() const;
};

#endif
