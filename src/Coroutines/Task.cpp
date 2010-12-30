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

Task::Task(TaskId taskId, Scheduler& scheduler)
                      : id(taskId), scheduler(scheduler)
{}

TaskId Task::getTaskId()
{
   return id;
}

void Task::signal()
{
   scheduler.taskDone(id);
   delete this;
}

Task::~Task()
{
}
