/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Task.h"
#include "Scheduler.h"
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_SCHEDULER

Task::Task(TaskId taskId, Scheduler& scheduler) :
   m_taskDeactivated(false),
   m_id(taskId),
   m_scheduler(scheduler)
{
   DEBUG("Creating task %d", m_id);
}

Task::Task(Task&& other) :
   m_taskDeactivated(other.m_taskDeactivated),
   m_id(other.m_id),
   m_scheduler(other.m_scheduler)
{
   other.m_taskDeactivated = true;
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
   m_taskDeactivated = true;
}
