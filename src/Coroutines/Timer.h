/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef TIMER_H
#define TIMER_H

#include "Thread.h"

/**
 * A Timer is a type of Thread that is used to track the passage of time in a
 * simple way. The Timer is supplied with a Task to signal when it reaches the
 * end of its countdown. The Timer is useful for delaying Threads, controlling
 * pacing, or creating countdowns.
 *
 * @author Noam Chitayat
 */
class Timer : public Thread
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
       * Resume this timer's countdown.
       *
       * @param timePassed The time that has passed since the last frame.
       */
      bool resume(long timePassed);

      /**
       * Destructor.
       */
      ~Timer();
};

#endif
