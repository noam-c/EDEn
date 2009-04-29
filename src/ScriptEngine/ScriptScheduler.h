#ifndef __SCRIPT_SCHEDULER_H_
#define __SCRIPT_SCHEDULER_H_

struct lua_State;

#include "TicketId.h"
#include <map>
#include <set>
#include <queue>

class ScriptScheduler
{  typedef std::map<TicketId, lua_State*> BlockList;
   typedef std::map<lua_State*, lua_State*> JoinList;
   typedef std::set<lua_State*> ReadyList;
   typedef std::queue<lua_State*> FinishedList;

   BlockList blockedThreads;
   JoinList joiningThreads;
   ReadyList readyThreads;
   ReadyList unstartedThreads;
   FinishedList finishedThreads;

   public:
      void block(lua_State* state, TicketId waitInstruction);
      void start(lua_State* state);
      void ready(TicketId readyInstruction);
      void join(lua_State* joiningState, lua_State* runningState);
      void finishJoin(lua_State* state);
      void finished(lua_State* state);

      void run();
};

#endif
