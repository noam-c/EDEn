/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Scheduler.h"
#include "Task.h"
#include "Coroutine.h"
#include "DebugUtils.h"
#include <tuple>
#include <utility>

#define DEBUG_FLAG DEBUG_SCHEDULER

Scheduler::Scheduler() :
   m_runningCoroutine(nullptr),
   m_nextId(0)
{
}

Scheduler::~Scheduler()
{
   // Since active tasks may be signalled by global functions (such as SDL callbacks),
   // do not delete them. However, deactivate them so that they don't try to signal this
   // (now deleted) scheduler.
   for(auto& taskIter : m_activeTasks)
   {
      auto& taskToDeactivate = taskIter.second;
      if(taskToDeactivate)
      {
         taskToDeactivate->signalSchedulerDestroyed();
      }
   }
}

void Scheduler::printFinishedQueue() const
{
   DEBUG("Finished Coroutine List:");
   DEBUG("---");
   for(const auto& coroutine : m_finishedCoroutines)
   {
      DEBUG("\t%d at address 0x%x", coroutine->getId(), coroutine.get());
   }
   DEBUG("---");
}

void Scheduler::start(const std::shared_ptr<Coroutine>& coroutine)
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
   return m_runningCoroutine != nullptr;
}

int Scheduler::block(const std::shared_ptr<Task>& pendingTask, int numResults)
{
   DEBUG("Blocking coroutine %d on task %d...", m_runningCoroutine->getId(), pendingTask->m_id);

   // Find the coroutine in the ready list
   if(m_readyCoroutines.find(m_runningCoroutine) != m_readyCoroutines.end())
   {
      // If the coroutine is in the ready list, push it into the finished coroutine list
      DEBUG("Putting coroutine %d in the finish list", m_runningCoroutine->getId());
      m_finishedCoroutines.push_back(m_runningCoroutine);
      printFinishedQueue();

      // Add the coroutine to the blocked list
      m_blockedCoroutines[pendingTask->m_id] = m_runningCoroutine;
   }
   else
   {
      T_T("Attempting to block a coroutine that isn't ready/running!");
   }

   DEBUG("Yielding: %d", m_runningCoroutine->getId());
   return m_runningCoroutine->yield(numResults);
}

std::shared_ptr<Task> Scheduler::createNewTask()
{
   TaskId taskId = m_nextId;
   auto& nextTask = m_activeTasks.emplace(std::piecewise_construct,
                                         std::forward_as_tuple(taskId),
                                         std::forward_as_tuple(new Task(taskId, *this))).first->second;
   m_nextId++;

   return nextTask;
}

void Scheduler::completeTask(TaskId finishedTaskId, std::unique_ptr<ICoroutineResults>&& taskResult)
{
   DEBUG("Task %d finished.", finishedTaskId);

   BlockList::iterator resumingCoroutineIterator = m_blockedCoroutines.find(finishedTaskId);
   if(resumingCoroutineIterator != m_blockedCoroutines.end())
   {
      auto& resumingCoroutine = resumingCoroutineIterator->second;
      DEBUG("Putting coroutine %d on resume list...", resumingCoroutine->getId());

      if (taskResult)
      {
         resumingCoroutine->setResults(std::move(taskResult));
      }

      // Put the resumed coroutine onto the unstarted stack
      m_unstartedCoroutines.insert(resumingCoroutine);

      // Remove the coroutine from the block list
      m_blockedCoroutines.erase(finishedTaskId);
   }

   TaskMap::iterator finishedTaskIterator = m_activeTasks.find(finishedTaskId);
   if(finishedTaskIterator != m_activeTasks.end())
   {
      // Once the task has been found in the active task list,
      // remove it and then place it in the finished task list
      m_tasksToDelete.push_back(finishedTaskIterator->second);
      m_activeTasks.erase(finishedTaskIterator);
   }
}

int Scheduler::join(const std::shared_ptr<Coroutine>& coroutine, int numResults)
{
   DEBUG("Joining coroutine %d on coroutine %d...", m_runningCoroutine->getId(), coroutine->getId());

   // Find the coroutine in the ready list
   if(m_readyCoroutines.find(m_runningCoroutine) != m_readyCoroutines.end())
   {
      // If the coroutine is in the ready list, push it into the finished coroutine list
      DEBUG("Putting coroutine %d in the finish list", m_runningCoroutine->getId());
      m_finishedCoroutines.push_back(m_runningCoroutine);
      printFinishedQueue();

      // Add the coroutine to the joining list
      m_joiningCoroutines[coroutine] = m_runningCoroutine;
   }
   else
   {
      T_T("Attempting to suspend a coroutine that isn't ready/running!");
   }

   DEBUG("Yielding: %d", m_runningCoroutine->getId());
   return m_runningCoroutine->yield(numResults);
}

void Scheduler::coroutineDone(const std::shared_ptr<Coroutine>& coroutine)
{
   // A coroutine has completed execution,
   // so check if anyone is waiting for it to finish
   auto& resumingCoroutine = m_joiningCoroutines[coroutine];

   if(resumingCoroutine)
   {
      // If there is such a coroutine, put it on the unstarted coroutine list again
      DEBUG("Putting coroutine %d on resume list...", resumingCoroutine->getId());
      m_unstartedCoroutines.insert(resumingCoroutine);

      // Clear the joining coroutine out of the joining list
      m_joiningCoroutines.erase(coroutine);
   }
}

void Scheduler::finished(const std::shared_ptr<Coroutine>& coroutine)
{
   // Check for any joins on this coroutine, then push it onto the finished
   // coroutine list
   coroutineDone(coroutine);
   DEBUG("Putting coroutine %d in the finish list", coroutine->getId());
   printFinishedQueue();
   m_finishedCoroutines.push_back(coroutine);
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
   m_runningCoroutine = nullptr;

   // If there are any finished/blocked coroutines,
   // remove them from the ready coroutine list
   for(auto& coroutine : m_finishedCoroutines)
   {
      DEBUG("Removing coroutine %d from ready list...", coroutine->getId());
      m_readyCoroutines.erase(coroutine);
   }

   m_finishedCoroutines.clear();
   m_tasksToDelete.clear();
}
