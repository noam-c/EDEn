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
   m_runningCoroutine(NULL),
   m_nextId(0)
{
}

Scheduler::~Scheduler()
{
   // Delete all the blocked coroutines.
   BlockList::iterator blockListIter;
   for(blockListIter = m_blockedCoroutines.begin(); blockListIter != m_blockedCoroutines.end(); ++blockListIter)
   {
      Coroutine* coroutineToDelete = blockListIter->second;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Delete all the coroutines waiting on a join.
   JoinList::iterator joinListIter;
   for(joinListIter = m_joiningCoroutines.begin(); joinListIter != m_joiningCoroutines.end(); ++joinListIter)
   {
      Coroutine* coroutineToDelete = joinListIter->second;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Delete all the unstarted coroutines.
   CoroutineList::iterator unstartedListIter;
   for(unstartedListIter = m_unstartedCoroutines.begin(); unstartedListIter != m_unstartedCoroutines.end(); ++unstartedListIter)
   {
      Coroutine* coroutineToDelete = *unstartedListIter;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Delete all the ready coroutines.
   CoroutineList::iterator readyListIter;
   for(readyListIter = m_readyCoroutines.begin(); readyListIter != m_readyCoroutines.end(); ++readyListIter)
   {
      Coroutine* coroutineToDelete = *readyListIter;
      if(coroutineToDelete != NULL)
      {
         delete coroutineToDelete;
      }
   }

   // Since active tasks may be signalled by global functions (such as SDL callbacks),
   // do not delete them. However, deactivate them so that they don't try to signal this
   // (now deleted) scheduler.
   TaskMap::iterator taskMapIter;
   for(taskMapIter = m_activeTasks.begin(); taskMapIter != m_activeTasks.end(); ++taskMapIter)
   {
      Task* taskToDeactivate = taskMapIter->second;
      if(taskToDeactivate != NULL)
      {
         taskToDeactivate->signalSchedulerDestroyed();
      }
   }

   // Delete all the finished coroutines.
   while(!m_finishedCoroutines.empty())
   {
      delete m_finishedCoroutines.front();
      m_finishedCoroutines.pop();
   }

   // Delete all the coroutines waiting to be destroyed.
   while(!m_coroutinesToDelete.empty())
   {
      delete m_coroutinesToDelete.front();
      m_coroutinesToDelete.pop();
   }

   // Delete all the tasks waiting to be destroyed.
   while(!m_tasksToDelete.empty())
   {
      delete m_tasksToDelete.front();
      m_tasksToDelete.pop();
   }
}

void Scheduler::printFinishedQueue()
{
   DEBUG("Finished Coroutine List:");
   DEBUG("---");
   const int queueSize = m_finishedCoroutines.size();
   for(int i = 0; i < queueSize; ++i)
   {
      Coroutine* t = m_finishedCoroutines.front();
      DEBUG("\t%d at address 0x%x", t->getId(), t);
      m_finishedCoroutines.pop();
      m_finishedCoroutines.push(t);
   }
   DEBUG("---");
}

void Scheduler::start(Coroutine* coroutine)
{
   DEBUG("Starting coroutine %d...", coroutine->getId());

   // Ensure that this coroutine is not already scheduled
   if(m_readyCoroutines.find(coroutine) == m_readyCoroutines.end()
      && m_unstartedCoroutines.find(coroutine) == m_unstartedCoroutines.end())
   {
      // Insert the coroutine into the unstarted coroutine list
      m_unstartedCoroutines.insert(coroutine);
   }
}

bool Scheduler::hasRunningCoroutine() const
{
   return m_runningCoroutine != NULL;
}

int Scheduler::block(Task* pendingTask)
{
   DEBUG("Blocking coroutine %d on task %d...", m_runningCoroutine->getId(), pendingTask->m_id);

   // Find the coroutine in the ready list
   if(m_readyCoroutines.find(m_runningCoroutine) != m_readyCoroutines.end())
   {
      // If the coroutine is in the ready list, push it into the finished coroutine list
      DEBUG("Putting coroutine %d in the finish list", m_runningCoroutine->getId());
      m_finishedCoroutines.push(m_runningCoroutine);
      printFinishedQueue();

      // Add the coroutine to the blocked list
      m_blockedCoroutines[pendingTask->m_id] = m_runningCoroutine;
   }
   else
   {
      T_T("Attempting to block a coroutine that isn't ready/running!");
   }

   DEBUG("Yielding: %d", m_runningCoroutine->getId());
   return m_runningCoroutine->yield();
}

Task* Scheduler::createNewTask()
{
   TaskId taskId = m_nextId;
   Task* nextTask = NULL;

   try
   {
      nextTask = new Task(taskId, *this);
      m_activeTasks[taskId] = nextTask;
      m_nextId++;
   }
   catch(std::exception& e)
   {
      if(nextTask != NULL)
      {
         delete nextTask;
         nextTask = NULL;
      }

      DEBUG("Failed to create new task: %s", e.what());
   }

   return nextTask;
}

void Scheduler::completeTask(TaskId finishedTaskId)
{
   DEBUG("Task %d finished.", finishedTaskId);

   BlockList::iterator resumingCoroutineIterator = m_blockedCoroutines.find(finishedTaskId);
   if(resumingCoroutineIterator != m_blockedCoroutines.end())
   {
      Coroutine* resumingCoroutine = resumingCoroutineIterator->second;
      DEBUG("Putting coroutine %d on resume list...", resumingCoroutine->getId());

      // Put the resumed coroutine onto the unstarted stack
      m_unstartedCoroutines.insert(resumingCoroutine);

      // Remove the coroutine from the block list
      m_blockedCoroutines[finishedTaskId] = NULL;
   }

   TaskMap::iterator finishedTaskIterator = m_activeTasks.find(finishedTaskId);
   if(finishedTaskIterator != m_activeTasks.end())
   {
      // Once the task has been found in the active task list,
      // remove it and then place it in the finished task list
      m_tasksToDelete.push(finishedTaskIterator->second);
      m_activeTasks.erase(finishedTaskIterator);
   }
}

int Scheduler::join(Coroutine* coroutine)
{
   DEBUG("Joining coroutine %d on coroutine %d...", m_runningCoroutine->getId(), coroutine->getId());

   // Find the coroutine in the ready list
   if(m_readyCoroutines.find(m_runningCoroutine) != m_readyCoroutines.end())
   {
      // If the coroutine is in the ready list, push it into the finished coroutine list
      DEBUG("Putting coroutine %d in the finish list", m_runningCoroutine->getId());
      m_finishedCoroutines.push(m_runningCoroutine);
      printFinishedQueue();

      // Add the coroutine to the joining list
      m_joiningCoroutines[coroutine] = m_runningCoroutine;
   }
   else
   {
      T_T("Attempting to suspend a coroutine that isn't ready/running!");
   }

   DEBUG("Yielding: %d", m_runningCoroutine->getId());
   return m_runningCoroutine->yield();
}

void Scheduler::coroutineDone(Coroutine* coroutine)
{
   // A coroutine has completed execution,
   // so check if anyone is waiting for it to finish
   Coroutine* resumingCoroutine = m_joiningCoroutines[coroutine];

   if(resumingCoroutine)
   {
      // If there is such a coroutine, put it on the unstarted coroutine list again
      DEBUG("Putting coroutine %d on resume list...", resumingCoroutine->getId());
      m_unstartedCoroutines.insert(resumingCoroutine);

      // Clear the joining coroutine out of the joining list
      m_joiningCoroutines[coroutine] = NULL;
   }
}

void Scheduler::finished(Coroutine* coroutine)
{
   // Check for any joins on this coroutine, then push it onto the finished
   // coroutine list
   coroutineDone(coroutine);
   DEBUG("Putting coroutine %d in the finish list", coroutine->getId());
   printFinishedQueue();
   m_finishedCoroutines.push(coroutine);
   m_coroutinesToDelete.push(coroutine);
}

void Scheduler::runCoroutines(long timePassed)
{
   // If there are any coroutines on the unstarted list, then put them all into
   // the ready list and clear the unstarted list
   if(!m_unstartedCoroutines.empty())
   {
      m_readyCoroutines.insert(m_unstartedCoroutines.begin(), m_unstartedCoroutines.end());
      m_unstartedCoroutines.clear();
   }

   // Run each coroutine until either it yields or finishes execution
   for(CoroutineList::iterator iter = m_readyCoroutines.begin(); iter != m_readyCoroutines.end(); ++iter)
   {
      // Set the running coroutine to the next coroutine
      m_runningCoroutine = *iter;

      try
      {
         // Run/resume the coroutine
         bool scriptIsFinished = m_runningCoroutine->resume(timePassed);
   
         if(scriptIsFinished)
         {
            finished(m_runningCoroutine);
         }
      }
      catch(Exception& e)
      {
         // This coroutine malfunctioned in some terminal way. We should not run it again.
         /** \todo These debug messages cause a segmentation fault when a map script throws an exception. */
         DEBUG("Coroutine failure encountered! Removing coroutine %d", m_runningCoroutine->getId());
         DEBUG("Reason: %s", e.getMessage().c_str());
         finished(*iter);
      }
   }

   // Clear the running coroutine since none are running now
   m_runningCoroutine = NULL;

   // If there are any finished/blocked coroutines,
   // remove them from the ready coroutine list
   while(!m_finishedCoroutines.empty())
   {
      Coroutine* coroutine = m_finishedCoroutines.front();
      DEBUG("Removing coroutine %d from ready list...", coroutine->getId());

      m_readyCoroutines.erase(coroutine);
      m_finishedCoroutines.pop();
   }

   // If there are any coroutines that are done and need deleting, delete them
   while(!m_coroutinesToDelete.empty())
   {
      Coroutine* coroutine = m_coroutinesToDelete.front();
      DEBUG("Deleting coroutine %d", coroutine->getId());
      delete coroutine;
      m_coroutinesToDelete.pop();
   }

   while(!m_tasksToDelete.empty())
   {
      Task* task = m_tasksToDelete.front();
      DEBUG("Deleting task %d", task->m_id);
      delete task;
      m_tasksToDelete.pop();
   }
}
