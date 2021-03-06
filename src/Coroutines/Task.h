/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef TASK_H
#define TASK_H

#include <utility>

#include "Coroutine.h"
#include "CoroutineResults.h"
#include "Scheduler.h"
#include "TaskId.h"

/**
 * A Task is a ticket container for engine instructions that occur across
 * many frames of logic. These tasks are handed to the Scheduler, which blocks
 * Coroutines waiting on the instruction and resumes them when the instruction is finished.
 *
 * @author Noam Chitayat
 */
class Task final
{
   friend class Scheduler;

   /** True iff the Scheduler managing this Task has been destroyed. */
   bool m_taskDeactivated = false;

   /** The unique identifier for this task */
   const TaskId m_id;

   /** The scheduler to signal when this task is finished */
   Scheduler& m_scheduler;

   /**
    *  Constructor. Assigns a new task ID and associates a Scheduler with the
    *  Task.
    *
    *  @param taskId The task ID for this task.
    *  @param scheduler The scheduler to signal when the task is completed.
    */
   Task(const TaskId taskId, Scheduler& scheduler);

   /**
    * Move constructor. Acquires the ID of the other tesk
    * and deactivates it so that there is at most one active
    * task for a given ID.
    */
   Task(Task&& other);

   /**
    * Because tasks have unique IDs, they cannot be copied.
    */
   Task(const Task& other) = delete;

   /**
    * Because tasks have unique IDs, they cannot be assigned.
    */
   Task& operator=(const Task& other) = delete;

   /**
    * Because tasks have unique IDs, they cannot be assigned.
    */
   Task& operator=(Task&& other) = delete;

   /**
    * Signals to a task that the Scheduler has been destroyed,
    * so that it doesn't try to operate on the Scheduler when it is signalled.
    */
   void signalSchedulerDestroyed();

   static std::unique_ptr<ICoroutineResults> makeResultsObject();

   template<typename ... Results> static std::unique_ptr<ICoroutineResults> makeResultsObject(Results&& ... results)
   {
      return std::unique_ptr<ICoroutineResults>(new CoroutineResults<Results ...>(std::forward<Results>(results)...));
   }

   public:
      /**
       * Signals the scheduler that the Task has finished, and destroys
       * this object.
       *
       * @param results The set of result values to pass to waiting coroutines.
       */
      template<typename ... Results> void complete(Results&& ... results)
      {
         if(!m_taskDeactivated)
         {
            m_scheduler.completeTask(m_id, Task::makeResultsObject(std::forward<Results>(results)...));
         }
      }

      /**
       * Destructor.
       */
      ~Task();
};

#endif
