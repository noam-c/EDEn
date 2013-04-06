/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Timer.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCHEDULER;

Timer::Timer(long timeLeft) :
   timeLeft(timeLeft)
{
   DEBUG("Timer ID is %d", getId());
}

Timer::~Timer()
{
   // Currently no extra cleanup necessary
}

bool Timer::resume(long timePassed)
{
   timeLeft -= timePassed;
   return timeLeft < 0;
}
