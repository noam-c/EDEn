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
   m_schedulerDestroyed(false),
   m_id(taskId),
   m_scheduler(scheduler)
{}

Task::~Task()
{
}

void Task::signalSchedulerDestroyed()
{
   m_schedulerDestroyed = true;
}

void Task::signal()
{
   if(!m_schedulerDestroyed)
   {
      m_scheduler.completeTask(m_id);
   }
   else
   {
      // If the Scheduler has been cleaned up,
      // the Task has to manage its own cleanup.

      /** \todo Remove this code when Tasks are managed using smart pointers. */
      delete this;
   }
}
