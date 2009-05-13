#include "Scheduler.h"
#include "Thread.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCRIPT_ENG;

Scheduler::Scheduler() : runningThread(NULL)
{
}

void Scheduler::start(Thread* thread)
{  DEBUG("Starting thread %d...", thread->getId());

   // Ensure that this thread is not already scheduled
   if(readyThreads.find(thread) == readyThreads.end()
      && unstartedThreads.find(thread) == unstartedThreads.end())
   {  // Insert the thread into the unstarted thread list
      unstartedThreads.insert(thread);
   }
}

bool Scheduler::hasRunningThread()
{  return runningThread;
}

int Scheduler::block(TicketId waitInstruction)
{  DEBUG("Blocking thread %d...", runningThread->getId());
   
   // Find the thread in the ready list
   if(readyThreads.find(runningThread) != readyThreads.end())
   {  // If the thread is in the ready list, push it into the finished thread list
      finishedThreads.push(runningThread);
      //readyThreads.erase(stateToBlock);

      // Add the thread to the blocked list
      blockedThreads[waitInstruction] = runningThread;
   }
   else
   {  T_T("Attempting to block a thread that isn't ready/running!");
   }

   DEBUG("Yielding: %d", runningThread->getId());
   return runningThread->yield();
}

void Scheduler::instructionDone(TicketId finishedInstruction)
{  Thread* resumingThread = blockedThreads[finishedInstruction];
   if(resumingThread)
   {  DEBUG("Putting thread %d on resume list...", resumingThread->getId());

      // Put the resumed thread onto the unstarted stack
      unstartedThreads.insert(resumingThread);

      // Remove the thread from the block list
      blockedThreads[finishedInstruction] = NULL;
   }
}

int Scheduler::join(Thread* thread)
{  DEBUG("Joining thread %d...", runningThread->getId());

   // Find the thread in the ready list
   if(readyThreads.find(runningThread) != readyThreads.end())
   {  // If the thread is in the ready list, push it into the finished thread list
      finishedThreads.push(runningThread);

      // Add the thread to the joining list
      joiningThreads[thread] = runningThread;
   }
   else
   {  T_T("Attempting to suspend a thread that isn't ready/running!");
   }

   DEBUG("Yielding: %d", runningThread->getId());
   return runningThread->yield();
}

void Scheduler::threadDone(Thread* thread)
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
   threadDone(thread);
   finishedThreads.push(thread);
   delete thread;
}

void Scheduler::runThreads(long timePassed)
{  // If there are any threads on the unstarted list, then put them all into
   // the ready list and clear the unstarted list
   if(!unstartedThreads.empty())
   {  readyThreads.insert(unstartedThreads.begin(), unstartedThreads.end());
      unstartedThreads.clear();
   }

   // Run each thread until either it yields or finishes execution
   for(ThreadList::iterator iter = readyThreads.begin(); iter != readyThreads.end(); ++iter)
   {  // Set the running thread to the next thread
      runningThread = *iter;

      // Run/resume the thread
      bool scriptIsFinished = (*iter)->resume(timePassed);

      if(scriptIsFinished)
      {  finished(*iter);
      }
   }

   // Clear the running thread since none are running now
   runningThread = NULL;

   // If there are any finished/blocked threads,
   // remove them from the ready thread list
   while(!finishedThreads.empty())
   {  Thread* thread = finishedThreads.front();
      DEBUG("Removing thread %d", thread->getId());

      readyThreads.erase(thread);
      finishedThreads.pop();
   }
}
