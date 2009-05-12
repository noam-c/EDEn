#include "Scheduler.h"
#include "ScriptEngine.h"
#include "Thread.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCRIPT_ENG;

void Scheduler::start(Thread* thread)
{  DEBUG("Starting thread %d...", thread->getId());

   // Ensure that this thread is not already scheduled
   if(readyThreads.find(thread) == readyThreads.end()
      && unstartedThreads.find(thread) == unstartedThreads.end())
   {  // Insert the thread into the unstarted thread list
      unstartedThreads.insert(thread);
   }
}

void Scheduler::block(Thread* thread, TicketId waitInstruction)
{  DEBUG("Blocking thread %d...", thread->getId());
   
   // Find the thread in the ready list
   if(readyThreads.find(thread) != readyThreads.end())
   {  // If the thread is in the ready list, push it into the finished thread list
      finishedThreads.push(thread);
      //readyThreads.erase(stateToBlock);

      // Add the thread to the blocked list
      blockedThreads[waitInstruction] = thread;
   }
   else
   {  T_T("Attempting to block a thread that isn't ready/running!");
   }

}

void Scheduler::ready(TicketId finishedInstruction)
{  Thread* resumingThread = blockedThreads[finishedInstruction];
   if(resumingThread)
   {  DEBUG("Putting thread %d on resume list...", resumingThread->getId());

      // Put the resumed thread onto the unstarted stack
      unstartedThreads.insert(resumingThread);

      // Remove the thread from the block list
      blockedThreads[finishedInstruction] = NULL;
   }
}

void Scheduler::join(Thread* joiningThread, Thread* runningThread)
{  DEBUG("Joining thread %d...", joiningThread->getId());

   // Find the thread in the ready list
   if(readyThreads.find(joiningThread) != readyThreads.end())
   {  // If the thread is in the ready list, push it into the finished thread list
      finishedThreads.push(joiningThread);
      //readyThreads.erase(stateToBlock);

      // Add the thread to the joining list
      joiningThreads[runningThread] = joiningThread;
   }
   else
   {  T_T("Attempting to join a thread that isn't ready/running!");
   }
}

void Scheduler::finishJoin(Thread* thread)
{  // A thread has completed execution,
   // so check if anyone is waiting for it to finish
   Thread* resumingThread = joiningThreads[thread];

   if(resumingThread)
   {  // If there is such a thread, put it on the unstarted thread list again
      DEBUG("Putting thread %d on resume list...", resumingThread->getId());
      unstartedThreads.insert(resumingThread);

      // Clear the joining thread out of the joining list
      joiningThreads[thread] = NULL;
   }
}

void Scheduler::finished(Thread* thread)
{  // Check for any joins on this thread, then push it onto the finished
   // thread list
   finishJoin(thread);
   finishedThreads.push(thread);
   delete thread;
}

void Scheduler::run(long timePassed)
{  // If there are any threads on the unstarted list, then put them all into
   // the ready list and clear the unstarted list
   if(!unstartedThreads.empty())
   {  readyThreads.insert(unstartedThreads.begin(), unstartedThreads.end());
      unstartedThreads.clear();
   }

   // Run each thread until either it yields or finishes execution
   for(ThreadList::iterator iter = readyThreads.begin(); iter != readyThreads.end(); ++iter)
   {  DEBUG("Resuming script %d...", (*iter)->getId());

      // Run/resume the thread
      bool scriptIsFinished = (*iter)->resume(timePassed);

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
   {  Thread* thread = finishedThreads.front();
      DEBUG("Removing thread %d", thread->getId());

      readyThreads.erase(thread);
      finishedThreads.pop();
   }
}
