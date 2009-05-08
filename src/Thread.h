#ifndef __THREAD_H_
#define __THREAD_H_

class Thread
{  static int numThreads;

   protected:
      int threadId;

   public:
      static int getNumThreads();
      Thread();
      int getId();
      virtual bool resume(long timePassed) = 0;
};

#endif
