/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Task.h"
#include "Scheduler.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCHEDULER;

/** The next available unique task identifier. */
static TaskId nextId = 0;

Task* Task::getNextTask(Scheduler& scheduler)
{
   return new Task(nextId++, scheduler);
}

Task::Task(TaskId taskId, Scheduler& scheduler) :
   id(taskId),
   scheduler(scheduler)
{}

Task::~Task()
{
}

TaskId Task::getTaskId() const
{
   return id;
}

void Task::signal()
{
   scheduler.taskDone(id);
   delete this;
}
