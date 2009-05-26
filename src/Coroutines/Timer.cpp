#include "Timer.h"
#include "Task.h"

Timer::Timer(Task* task, long timeLeft) : timerTask(task), timeLeft(timeLeft)
{}

bool Timer::resume(long timePassed)
{  timeLeft -= timePassed;

   if(timeLeft < 0)
   {  timerTask->signal();
      return true;
   }

   return false;
}
