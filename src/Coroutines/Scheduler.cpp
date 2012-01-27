/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Scheduler.h"
#include "Task.h"
#include "Thread.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCHEDULER;

Scheduler::Scheduler() : runningThread(NULL)
{
}

void Scheduler::printFinishedQueue()
{
   DEBUG("Finished Thread List:");
   DEBUG("---");
   const int queueSize = finishedThreads.size();
   for(int i = 0; i < queueSize; ++i)
   {
      Thread* t = finishedThreads.front();
      DEBUG("\t%d at address 0x%x", t->getId(), t);
      finishedThreads.pop();
      finishedThreads.push(t);
   }
   DEBUG("---");
}

void Scheduler::start(Thread* thread)
{
   DEBUG("Starting thread %d...", thread->getId());

   // Ensure that this thread is not already scheduled
   if(readyThreads.find(thread) == readyThreads.end()
      && unstartedThreads.find(thread) == unstartedThreads.end())
   {
      // Insert the thread into the unstarted thread list
      unstartedThreads.insert(thread);
   }
}

bool Scheduler::hasRunningThread()
{
   return runningThread;
}

int Scheduler::block(Task* pendingTask)
{
   DEBUG("Blocking thread %d on task %d...", runningThread->getId(), pendingTask->getTaskId());

   // Find the thread in the ready list
   if(readyThreads.find(runningThread) != readyThreads.end())
   {
      // If the thread is in the ready list, push it into the finished thread list
      DEBUG("Putting thread %d in the finish list", runningThread->getId());
      finishedThreads.push(runningThread);
      printFinishedQueue();

      // Add the thread to the blocked list
      blockedThreads[pendingTask->getTaskId()] = runningThread;
   }
   else
   {
      T_T("Attempting to block a thread that isn't ready/running!");
   }

   DEBUG("Yielding: %d", runningThread->getId());
   return runningThread->yield();
}

void Scheduler::taskDone(TaskId finishedTask)
{
   DEBUG("Task %d finished.", finishedTask);

   Thread* resumingThread = blockedThreads[finishedTask];
   if(resumingThread)
   {
      DEBUG("Putting thread %d on resume list...", resumingThread->getId());

      // Put the resumed thread onto the unstarted stack
      unstartedThreads.insert(resumingThread);

      // Remove the thread from the block list
      blockedThreads[finishedTask] = NULL;
   }
}

int Scheduler::join(Thread* thread)
{
   DEBUG("Joining thread %d on thread %d...", runningThread->getId(), thread->getId());

   // Find the thread in the ready list
   if(readyThreads.find(runningThread) != readyThreads.end())
   {
      // If the thread is in the ready list, push it into the finished thread list
      DEBUG("Putting thread %d in the finish list", runningThread->getId());
      finishedThreads.push(runningThread);
      printFinishedQueue();

      // Add the thread to the joining list
      joiningThreads[thread] = runningThread;
   }
   else
   {
      T_T("Attempting to suspend a thread that isn't ready/running!");
   }

   DEBUG("Yielding: %d", runningThread->getId());
   return runningThread->yield();
}

void Scheduler::threadDone(Thread* thread)
{
   // A thread has completed execution,
   // so check if anyone is waiting for it to finish
   Thread* resumingThread = joiningThreads[thread];

   if(resumingThread)
   {
      // If there is such a thread, put it on the unstarted thread list again
      DEBUG("Putting thread %d on resume list...", resumingThread->getId());
      unstartedThreads.insert(resumingThread);

      // Clear the joining thread out of the joining list
      joiningThreads[thread] = NULL;
   }
}

void Scheduler::finished(Thread* thread)
{
   // Check for any joins on this thread, then push it onto the finished
   // thread list
   threadDone(thread);
   DEBUG("Putting thread %d in the finish list", thread->getId());
   printFinishedQueue();
   finishedThreads.push(thread);
   deletedThreads.push(thread);
}

void Scheduler::runThreads(long timePassed)
{
   // If there are any threads on the unstarted list, then put them all into
   // the ready list and clear the unstarted list
   if(!unstartedThreads.empty())
   {
      readyThreads.insert(unstartedThreads.begin(), unstartedThreads.end());
      unstartedThreads.clear();
   }

   // Run each thread until either it yields or finishes execution
   for(ThreadList::iterator iter = readyThreads.begin(); iter != readyThreads.end(); ++iter)
   {
      // Set the running thread to the next thread
      runningThread = *iter;

      try
      {
         // Run/resume the thread
         bool scriptIsFinished = runningThread->resume(timePassed);
   
         if(scriptIsFinished)
         {
            finished(runningThread);
         }
      }
      catch(Exception& e)
      {
         // This coroutine malfunctioned in some terminal way. We should not run it again.
         /** \todo These debug messages cause a segmentation fault when a map script throws an exception. */
         DEBUG("Thread failure encountered! Removing thread %d", runningThread->getId());
         DEBUG("Reason: %s", e.getMessage().c_str());
         finished(*iter);
      }
   }

   // Clear the running thread since none are running now
   runningThread = NULL;

   // If there are any finished/blocked threads,
   // remove them from the ready thread list
   while(!finishedThreads.empty())
   {
      Thread* thread = finishedThreads.front();
      DEBUG("Removing thread %d from ready list...", thread->getId());

      readyThreads.erase(thread);
      finishedThreads.pop();
   }

   // If there are any threads that are done and need deleting, delete them
   while(!deletedThreads.empty())
   {
      Thread* thread = deletedThreads.front();
      DEBUG("Deleting thread %d", thread->getId());
      delete thread;
      deletedThreads.pop();
   }
}

Scheduler::~Scheduler()
{
   // Delete all the blocked threads.
   BlockList::iterator blockListIter;
   for(blockListIter = blockedThreads.begin(); blockListIter != blockedThreads.end(); ++blockListIter)
   {
      Thread* threadToDelete = blockListIter->second;
      if(threadToDelete != NULL)
      {
         delete threadToDelete;
      }
   }

   // Delete all the threads waiting on a join.
   JoinList::iterator joinListIter;
   for(joinListIter = joiningThreads.begin(); joinListIter != joiningThreads.end(); ++joinListIter)
   {
      Thread* threadToDelete = joinListIter->second;
      if(threadToDelete != NULL)
      {
         delete threadToDelete;
      }
   }

   // Delete all the unstarted threads.
   ThreadList::iterator unstartedListIter;
   for(unstartedListIter = unstartedThreads.begin(); unstartedListIter != unstartedThreads.end(); ++unstartedListIter)
   {
      Thread* threadToDelete = *unstartedListIter;
      if(threadToDelete != NULL)
      {
         delete threadToDelete;
      }
   }

   // Delete all the ready threads.
   ThreadList::iterator readyListIter;
   for(readyListIter = readyThreads.begin(); readyListIter != readyThreads.end(); ++readyListIter)
   {
      Thread* threadToDelete = *readyListIter;
      if(threadToDelete != NULL)
      {
         delete threadToDelete;
      }
   }

   // Delete all the finished threads.
   while(!finishedThreads.empty())
   {
      delete finishedThreads.front();
      finishedThreads.pop();
   }

   // Delete all the threads waiting to be destroyed.
   while(!deletedThreads.empty())
   {
      delete deletedThreads.front();
      deletedThreads.pop();
   }
}
