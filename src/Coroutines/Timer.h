#ifndef __TIMER_H_
#define __TIMER_H_

#include "Thread.h"

class Task;

class Timer : public Thread
{  long timeLeft;
   Task* timerTask;
   
   public:
      Timer(Task* task, long time);
      bool resume(long timePassed);
};

#endif
