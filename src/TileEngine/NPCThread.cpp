#include "NPC.h"
#include "NPCScript.h"
#include "NPCThread.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

NPCThread::NPCThread(NPC* npc, NPCScript* script)
                          : npc(npc), npcScript(script), finished(false)
{
}

bool NPCThread::resume(long timePassed)
{  if(finished) return true;
   DEBUG("NPC Thread %d running.", getId());
   if(npcScript->isRunning())
   {  DEBUG("NPC Thread %d resuming running script.", getId());
      npcScript->resume(timePassed);
   }
   else
   {  if(npc->isIdle())
      {  DEBUG("NPC Thread %d idling.", getId());
         npcScript->callFunction("idle");
      }
   }

   return false;
}

int NPCThread::yield()
{  return npcScript->yield();
}

void NPCThread::activate()
{  npcScript->callFunction("activate");
}

void NPCThread::finish()
{  finished = true;
}
