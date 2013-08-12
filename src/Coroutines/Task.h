/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef TASK_H
#define TASK_H

#include "TaskId.h"

class Scheduler;

/**
 * A Task is a ticket container for engine instructions that occur across
 * many frames of logic. These tasks are handed to the Scheduler, which blocks
 * Coroutines waiting on the instruction and resumes them when the instruction is finished.
 *
 * @author Noam Chitayat
 */
class Task
{
   friend class Scheduler;

   /** True iff the Scheduler managing this Task has been destroyed. */
   bool m_schedulerDestroyed;

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
    * Destructor.
    */
   ~Task();

   /**
    * Signals to a task that the Scheduler has been destroyed,
    * so that it doesn't try to operate on the Scheduler when it is signalled.
    */
   void signalSchedulerDestroyed();

   public:
      /**
       *  Signals the scheduler that the Task has finished, and destroys
       *  this object. (Yes, this method triggers a self-destruct)
       */
      void signal();
};

#endif
