/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Scheduler.h"
#include "Task.h"
#include "Coroutine.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCHEDULER;

Scheduler::Scheduler() :
   runningCoroutine(NULL)
{
}

Scheduler::~Scheduler()
{
   // Delete all the blocked coroutines.
   BlockList::iterator blockListIter;
   for(blockListIter = blockedCoroutines.begin(); blockListIter != blockedCoroutines.end(); ++blockListIter)
   {
      Coroutine* coroutineToDelete = blockListIter->second;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Delete all the coroutines waiting on a join.
   JoinList::iterator joinListIter;
   for(joinListIter = joiningCoroutines.begin(); joinListIter != joiningCoroutines.end(); ++joinListIter)
   {
      Coroutine* coroutineToDelete = joinListIter->second;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Delete all the unstarted coroutines.
   CoroutineList::iterator unstartedListIter;
   for(unstartedListIter = unstartedCoroutines.begin(); unstartedListIter != unstartedCoroutines.end(); ++unstartedListIter)
   {
      Coroutine* coroutineToDelete = *unstartedListIter;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Delete all the ready coroutines.
   CoroutineList::iterator readyListIter;
   for(readyListIter = readyCoroutines.begin(); readyListIter != readyCoroutines.end(); ++readyListIter)
   {
      Coroutine* coroutineToDelete = *readyListIter;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Delete all the finished coroutines.
   while(!finishedCoroutines.empty())
   {
      delete finishedCoroutines.front();
      finishedCoroutines.pop();
   }

   // Delete all the coroutines waiting to be destroyed.
   while(!deletedCoroutines.empty())
   {
      delete deletedCoroutines.front();
      deletedCoroutines.pop();
   }
}

void Scheduler::printFinishedQueue()
{
   DEBUG("Finished Coroutine List:");
   DEBUG("---");
   const int queueSize = finishedCoroutines.size();
   for(int i = 0; i < queueSize; ++i)
   {
      Coroutine* t = finishedCoroutines.front();
      DEBUG("\t%d at address 0x%x", t->getId(), t);
      finishedCoroutines.pop();
      finishedCoroutines.push(t);
   }
   DEBUG("---");
}

void Scheduler::start(Coroutine* coroutine)
{
   DEBUG("Starting coroutine %d...", coroutine->getId());

   // Ensure that this coroutine is not already scheduled
   if(readyCoroutines.find(coroutine) == readyCoroutines.end()
      && unstartedCoroutines.find(coroutine) == unstartedCoroutines.end())
   {
      // Insert the coroutine into the unstarted coroutine list
      unstartedCoroutines.insert(coroutine);
   }
}

bool Scheduler::hasRunningCoroutine() const
{
   return runningCoroutine != NULL;
}

int Scheduler::block(Task* pendingTask)
{
   DEBUG("Blocking coroutine %d on task %d...", runningCoroutine->getId(), pendingTask->getTaskId());

   // Find the coroutine in the ready list
   if(readyCoroutines.find(runningCoroutine) != readyCoroutines.end())
   {
      // If the coroutine is in the ready list, push it into the finished coroutine list
      DEBUG("Putting coroutine %d in the finish list", runningCoroutine->getId());
      finishedCoroutines.push(runningCoroutine);
      printFinishedQueue();

      // Add the coroutine to the blocked list
      blockedCoroutines[pendingTask->getTaskId()] = runningCoroutine;
   }
   else
   {
      T_T("Attempting to block a coroutine that isn't ready/running!");
   }

   DEBUG("Yielding: %d", runningCoroutine->getId());
   return runningCoroutine->yield();
}

void Scheduler::taskDone(TaskId finishedTask)
{
   DEBUG("Task %d finished.", finishedTask);

   Coroutine* resumingCoroutine = blockedCoroutines[finishedTask];
   if(resumingCoroutine)
   {
      DEBUG("Putting coroutine %d on resume list...", resumingCoroutine->getId());

      // Put the resumed coroutine onto the unstarted stack
      unstartedCoroutines.insert(resumingCoroutine);

      // Remove the coroutine from the block list
      blockedCoroutines[finishedTask] = NULL;
   }
}

int Scheduler::join(Coroutine* coroutine)
{
   DEBUG("Joining coroutine %d on coroutine %d...", runningCoroutine->getId(), coroutine->getId());

   // Find the coroutine in the ready list
   if(readyCoroutines.find(runningCoroutine) != readyCoroutines.end())
   {
      // If the coroutine is in the ready list, push it into the finished coroutine list
      DEBUG("Putting coroutine %d in the finish list", runningCoroutine->getId());
      finishedCoroutines.push(runningCoroutine);
      printFinishedQueue();

      // Add the coroutine to the joining list
      joiningCoroutines[coroutine] = runningCoroutine;
   }
   else
   {
      T_T("Attempting to suspend a coroutine that isn't ready/running!");
   }

   DEBUG("Yielding: %d", runningCoroutine->getId());
   return runningCoroutine->yield();
}

void Scheduler::coroutineDone(Coroutine* coroutine)
{
   // A coroutine has completed execution,
   // so check if anyone is waiting for it to finish
   Coroutine* resumingCoroutine = joiningCoroutines[coroutine];

   if(resumingCoroutine)
   {
      // If there is such a coroutine, put it on the unstarted coroutine list again
      DEBUG("Putting coroutine %d on resume list...", resumingCoroutine->getId());
      unstartedCoroutines.insert(resumingCoroutine);

      // Clear the joining coroutine out of the joining list
      joiningCoroutines[coroutine] = NULL;
   }
}

void Scheduler::finished(Coroutine* coroutine)
{
   // Check for any joins on this coroutine, then push it onto the finished
   // coroutine list
   coroutineDone(coroutine);
   DEBUG("Putting coroutine %d in the finish list", coroutine->getId());
   printFinishedQueue();
   finishedCoroutines.push(coroutine);
   deletedCoroutines.push(coroutine);
}

void Scheduler::runCoroutines(long timePassed)
{
   // If there are any coroutines on the unstarted list, then put them all into
   // the ready list and clear the unstarted list
   if(!unstartedCoroutines.empty())
   {
      readyCoroutines.insert(unstartedCoroutines.begin(), unstartedCoroutines.end());
      unstartedCoroutines.clear();
   }

   // Run each coroutine until either it yields or finishes execution
   for(CoroutineList::iterator iter = readyCoroutines.begin(); iter != readyCoroutines.end(); ++iter)
   {
      // Set the running coroutine to the next coroutine
      runningCoroutine = *iter;

      try
      {
         // Run/resume the coroutine
         bool scriptIsFinished = runningCoroutine->resume(timePassed);
   
         if(scriptIsFinished)
         {
            finished(runningCoroutine);
         }
      }
      catch(Exception& e)
      {
         // This coroutine malfunctioned in some terminal way. We should not run it again.
         /** \todo These debug messages cause a segmentation fault when a map script throws an exception. */
         DEBUG("Coroutine failure encountered! Removing coroutine %d", runningCoroutine->getId());
         DEBUG("Reason: %s", e.getMessage().c_str());
         finished(*iter);
      }
   }

   // Clear the running coroutine since none are running now
   runningCoroutine = NULL;

   // If there are any finished/blocked coroutines,
   // remove them from the ready coroutine list
   while(!finishedCoroutines.empty())
   {
      Coroutine* coroutine = finishedCoroutines.front();
      DEBUG("Removing coroutine %d from ready list...", coroutine->getId());

      readyCoroutines.erase(coroutine);
      finishedCoroutines.pop();
   }

   // If there are any coroutines that are done and need deleting, delete them
   while(!deletedCoroutines.empty())
   {
      Coroutine* coroutine = deletedCoroutines.front();
      DEBUG("Deleting coroutine %d", coroutine->getId());
      delete coroutine;
      deletedCoroutines.pop();
   }
}
