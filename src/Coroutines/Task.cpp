/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Task.h"
#include "Scheduler.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCHEDULER;

Task::Task(TaskId taskId, Scheduler& scheduler) :
   schedulerDestroyed(false),
   id(taskId),
   scheduler(scheduler)
{}

Task::~Task()
{
}

void Task::signalSchedulerDestroyed()
{
   schedulerDestroyed = true;
}

void Task::signal()
{
   if(!schedulerDestroyed)
   {
      scheduler.completeTask(id);
   }
   else
   {
      // If the Scheduler has been cleaned up,
      // the Task has to manage its own cleanup.

      /** \todo Remove this code when Tasks are managed using smart pointers. */
      delete this;
   }
}
