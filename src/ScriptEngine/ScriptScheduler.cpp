#include "ScriptScheduler.h"
#include "ScriptEngine.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCRIPT_ENG;

void ScriptScheduler::start(lua_State* state)
{  DEBUG("Starting thread...");
   DEBUG((int)state);

   // Ensure that this thread is not already scheduled
   if(readyThreads.find(state) == readyThreads.end()
      && unstartedThreads.find(state) == unstartedThreads.end())
   {  // Insert the thread into the unstarted thread list
      unstartedThreads.insert(state);
   }
}

void ScriptScheduler::block(lua_State* state, TicketId waitInstruction)
{  DEBUG("Blocking thread...");
   DEBUG((int)state);
   
   // Find the thread in the ready list
   if(readyThreads.find(state) != readyThreads.end())
   {  // If the thread is in the ready list, push it into the finished thread list
      finishedThreads.push(state);
      //readyThreads.erase(stateToBlock);

      // Add the thread to the blocked list
      blockedThreads[waitInstruction] = state;
   }
   else
   {  T_T("Attempting to block a thread that isn't ready/running!");
   }

}

void ScriptScheduler::ready(TicketId readyInstruction)
{  lua_State* resumingState = blockedThreads[readyInstruction];
   if(resumingState)
   {  DEBUG("Putting thread on resume list...");
      DEBUG((int)resumingState);

      // Put the resumed thread onto the unstarted stack
      unstartedThreads.insert(resumingState);

      // Remove the thread from the block list
      blockedThreads[readyInstruction] = NULL;
   }
}

void ScriptScheduler::join(lua_State* joiningState, lua_State* runningState)
{  DEBUG("Joining thread...");
   DEBUG((int)joiningState);

   // Find the thread in the ready list
   if(readyThreads.find(joiningState) != readyThreads.end())
   {  // If the thread is in the ready list, push it into the finished thread list
      finishedThreads.push(joiningState);
      //readyThreads.erase(stateToBlock);

      // Add the thread to the joining list
      joiningThreads[runningState] = joiningState;
   }
   else
   {  T_T("Attempting to join a thread that isn't ready/running!");
   }
}

void ScriptScheduler::finishJoin(lua_State* state)
{  // A thread has completed execution,
   // so check if anyone is waiting for it to finish
   lua_State* resumingState = joiningThreads[state];

   if(resumingState)
   {  // If there is such a thread, put it on the unstarted thread list again
      DEBUG("Putting thread on resume list...");
      DEBUG((int)resumingState);
      unstartedThreads.insert(resumingState);

      // Clear the joining thread out of the joining list
      joiningThreads[state] = NULL;
   }
}

void ScriptScheduler::finished(lua_State* state)
{  // Check for any joins on this thread, then push it onto the finished
   // thread list
   finishJoin(state);
   finishedThreads.push(state);
}

void ScriptScheduler::run()
{  // If there are any threads on the unstarted list, then put them all into
   // the ready list and clear the unstarted list
   if(!unstartedThreads.empty())
   {  readyThreads.insert(unstartedThreads.begin(), unstartedThreads.end());
      unstartedThreads.clear();
   }

   // Run each thread until either it yields or finishes execution
   for(ReadyList::iterator iter = readyThreads.begin(); iter != readyThreads.end(); ++iter)
   {  DEBUG("Resuming script...");
      DEBUG((int)*iter);

      // Run/resume the thread
      bool scriptIsFinished = ScriptEngine::getInstance()->resumeScript(*iter);

      if(scriptIsFinished)
      {  finished(*iter);
         DEBUG("Script finished.");
      }
      else
      {  DEBUG("Script yielded.");
      }
   }

   // If there are any finished/blocked threads,
   // remove them from the ready thread list
   while(!finishedThreads.empty())
   {  lua_State* state = finishedThreads.front();
      DEBUG("Removing thread");
      DEBUG((int)state);

      readyThreads.erase(state);
      finishedThreads.pop();
   }
}
