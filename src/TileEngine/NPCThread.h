#ifndef __NPC_THREAD_H_
#define __NPC_THREAD_H_

#include "Thread.h"

class NPC;
class NPCScript;

/**
 * \todo Merge this entire class into NPCScript.
 */
class NPCThread : public Thread
{  NPC* npc;
   NPCScript* npcScript;
   bool finished;

   public:
      NPCThread(NPC* npc, NPCScript* script);
      bool resume(long timePassed);
      int yield();
      void activate();
      void finish();
};

#endif
