/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Timer.h"
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_SCHEDULER

Timer::Timer(long timeLeft) :
   m_timeLeft(timeLeft)
{
   DEBUG("Timer ID is %d", getId());
}

bool Timer::resume(long timePassed)
{
   m_timeLeft -= timePassed;
   return m_timeLeft < 0;
}
