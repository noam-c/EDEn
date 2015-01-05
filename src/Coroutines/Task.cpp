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
{
   DEBUG("Creating task %d", m_id);
}

Task::~Task()
{
   DEBUG("Deleting task %d", m_id);
}

std::unique_ptr<ICoroutineResults> Task::makeResultsObject()
{
   return nullptr;
}

void Task::signalSchedulerDestroyed()
{
   m_schedulerDestroyed = true;
}
