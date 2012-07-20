/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TASK_H
#define TASK_H

#include "TaskId.h"

class Scheduler;

/**
 * A Task is a ticket container for engine instructions that occur across
 * many frames of logic. These tasks are handed to the Scheduler, which blocks
 * Threads waiting on the instruction and resumes them when the instruction is finished.
 *
 * @author Noam Chitayat
 */
class Task
{
   /** The unique identifier for this task */
   TaskId id;
   
   /** The scheduler to signal when this task is finished */
   Scheduler& scheduler;

   /**
    *  Constructor. Assigns a new task ID and associates a Scheduler with the
    *  Task.
    *
    *  @param taskId The task ID for this task.
    *  @param scheduler The scheduler to signal when the task is completed.
    */
   Task(TaskId taskId, Scheduler& scheduler);

   /**
    * Destructor. Private so that Tasks (which self-destruct)
    * can't get declared on the stack.
    */
   ~Task();

   public:
      /**
       *  Create and return a new task.
       *
       *  @param scheduler The scheduler to signal when the new task completes.
       *  @return A newly created task associated with the given scheduler.
       */
      static Task* getNextTask(Scheduler& scheduler);

      /**
       *  @return the unique identifier of this Task.
       */
      TaskId getTaskId() const;

      /**
       *  Signals the scheduler that the Task has finished, and destroys
       *  this object. (Yes, this method triggers a self-destruct)
       */
      void signal();
};

#endif
