/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TIMER_H
#define TIMER_H

#include "Coroutine.h"

/**
 * A Timer is a type of Coroutine that is used to track the passage of time in a
 * simple way. The Timer is supplied with a Task to signal when it reaches the
 * end of its countdown. The Timer is useful for delaying Coroutines, controlling
 * pacing, or creating countdowns.
 *
 * @author Noam Chitayat
 */
class Timer : public Coroutine
{
   /** The amount of time left before this timer is finished. */
   long timeLeft;

   public:
      /**
       * Constructor.
       *
       * @param time The amount of time this timer initializes with.
       */
      Timer(long time);

      /**
       * Destructor.
       */
      ~Timer();

      /**
       * Resume this timer's countdown.
       *
       * @param timePassed The time that has passed since the last frame.
       */
      bool resume(long timePassed);
};

#endif
