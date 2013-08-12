/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Timer.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCHEDULER;

Timer::Timer(long timeLeft) :
   m_timeLeft(timeLeft)
{
   DEBUG("Timer ID is %d", getId());
}

Timer::~Timer()
{
   // Currently no extra cleanup necessary
}

bool Timer::resume(long timePassed)
{
   m_timeLeft -= timePassed;
   return m_timeLeft < 0;
}
