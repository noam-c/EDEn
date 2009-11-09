#include "Timer.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCHEDULER;

Timer::Timer(long timeLeft) : timeLeft(timeLeft)
{
   DEBUG("Timer ID is %d", getId());
}

bool Timer::resume(long timePassed)
{
   timeLeft -= timePassed;
   return timeLeft < 0;
}

Timer::~Timer()
{
   // Currently no extra cleanup necessary
}
