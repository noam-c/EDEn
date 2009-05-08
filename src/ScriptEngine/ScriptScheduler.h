#ifndef __SCRIPT_SCHEDULER_H_
#define __SCRIPT_SCHEDULER_H_

class Thread;

#include "TicketId.h"
#include <map>
#include <set>
#include <queue>

class ScriptScheduler
{  typedef std::map<TicketId, Thread*> BlockList;
   typedef std::map<Thread*, Thread*> JoinList;
   typedef std::set<Thread*> ReadyList;
   typedef std::queue<Thread*> FinishedList;

   BlockList blockedThreads;
   JoinList joiningThreads;
   ReadyList readyThreads;
   ReadyList unstartedThreads;
   FinishedList finishedThreads;

   public:
      void block(Thread* state, TicketId waitInstruction);
      void start(Thread* state);
      void ready(TicketId readyInstruction);
      void join(Thread* joiningState, Thread* runningState);
      void finishJoin(Thread* state);
      void finished(Thread* state);

      void run(long timePassed);
};

#endif
