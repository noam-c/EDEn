#ifndef __TIMER_H_
#define __TIMER_H_

#include "Thread.h"

class Task;

/**
 * A Timer is a type of Thread that is used to track the passage of time in a
 * simple way. The Timer is supplied with a Task to signal when it reaches the
 * end of its countdown. The Timer is useful for delaying Threads, controlling
 * pacing, or creating countdowns.
 *
 * @author Noam Chitayat
 */
class Timer : public Thread
{  long timeLeft;
   Task* timerTask;
   
   public:
      Timer(Task* task, long time);
      bool resume(long timePassed);
};

#endif
