/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SCRIPT_SCHEDULER_H
#define SCRIPT_SCHEDULER_H

#include "TaskId.h"

#include <map>
#include <memory>
#include <set>
#include <deque>

class Task;
class Coroutine;
class ICoroutineResults;

/**
 * A scheduler for scheduling, resuming and blocking a set of Coroutines.
 * Each call to run resumes each ready coroutine, one by one, and retrieves a
 * return value indicating whether or not the Coroutine has yielded or completed.
 *
 * Objects that work with the coroutines can request that the scheduler block them
 * until completion of a task or until another Coroutine has completed execution.
 *
 * @author Noam Chitayat
 */
class Scheduler
{
   /** A list of coroutines. */
   typedef std::set<std::shared_ptr<Coroutine>> CoroutineList;

   /** A queue of coroutines. */
   typedef std::deque<std::shared_ptr<Coroutine>> CoroutineQueue;

   /** A list of blocked coroutines, indexed by the TaskId on which they are waiting */
   typedef std::map<TaskId, std::shared_ptr<Coroutine>> BlockList;

   /** A list of joining/waiting coroutines, indexed by the Coroutine on which they are waiting */
   typedef std::map<std::shared_ptr<Coroutine>, std::shared_ptr<Coroutine>> JoinList;

   /** A list of Tasks, indexed by their Task IDs */
   typedef std::map<TaskId, std::shared_ptr<Task>> TaskMap;

   /** A queue of Tasks */
   typedef std::deque<std::shared_ptr<Task>> TaskQueue;

   /** The list of currently blocked coroutines in this scheduler */
   BlockList m_blockedCoroutines;

   /** The list of currently joining/waiting coroutines in this scheduler */
   JoinList m_joiningCoroutines;

   /** The list of newly readied coroutines to be added to the ready list in the next frame. */
   CoroutineList m_unstartedCoroutines;

   /** The list of ready coroutines to be resumed during the next frame. */
   CoroutineList m_readyCoroutines;

   /** A list of coroutines to remove from the ready list after a frame. */
   CoroutineQueue m_finishedCoroutines;

   /** A list of the active tasks created and managed by this Scheduler. */
   TaskMap m_activeTasks;

   /** A list of coroutines to delete after a frame. */
   CoroutineQueue m_coroutinesToDelete;

   /** A list of tasks to delete after a frame. */
   TaskQueue m_tasksToDelete;

   /** The currently running coroutine */
   std::shared_ptr<Coroutine> m_runningCoroutine;

   /** The next available unique task identifier. */
   TaskId m_nextId;

   /**
    * Signal that a Coroutine has run to completion so that waiting Coroutines
    * can be unblocked.
    *
    * @param coroutine The coroutine that has completed execution
    */
   void coroutineDone(const std::shared_ptr<Coroutine>& coroutine);

   public:
      /**
       * Constructor. Initializes member variables.
       */
      Scheduler();

      /**
       * @return true iff there is a Coroutine currently running in the Scheduler
       */
      bool hasRunningCoroutine() const;

      /**
       * Block a Coroutine on a specified instruction TicketId. Coroutine will be
       * readied again when the instruction is finished executing.
       *
       * @param task The task upon which the coroutine is waiting.
       *
       * @return a yield code from the Coroutine being blocked
       */
      int block(const std::shared_ptr<Task>& task, int numResults = 0);

      /**
       * Add a Coroutine to the scheduler by enqueuing it to be started on the next run.
       *
       * @param coroutine The coroutine to start.
       */
      void start(const std::shared_ptr<Coroutine>& coroutine);

      /**
       *  Create and return a new task.
       *
       *  @return A newly created task associated with this scheduler.
       */
      std::shared_ptr<Task> createNewTask();

      /**
       * Signals that an instruction has been completed so that the scheduler
       * can unblock any waiting Coroutines.
       *
       * @param finishedTaskId The ID of the instruction that has been finished.
       */
      void completeTask(TaskId finishedTaskId, std::unique_ptr<ICoroutineResults>&& taskResult);

      /**
       * Block a Coroutine and make it wait until another Coroutine has finished
       * executing.
       *
       * @param runningCoroutine The Coroutine on which the joining Coroutine is waiting.
       *
       * @return a yield code from the Coroutine being blocked
       */
      int join(const std::shared_ptr<Coroutine>& runningCoroutine, int numResults = 0);

      /**
       * Signal that a Coroutine has been finished and destroy the Coroutine.
       */
      void finished(const std::shared_ptr<Coroutine>& coroutine);

      /**
       * Print the contents of the finished coroutine list to debug output.
       */
      void printFinishedQueue() const;

      /**
       * A scheduler run resumes each Coroutine in order and allows them to execute
       * until either completion or yielding.
       *
       * @param timePassed The amount of time that has passed since the last frame.
       *                   (roughly speaking, the amount of time since the last run)
       */
      void runCoroutines(long timePassed);

      /**
       * Destrutor.
       */
      ~Scheduler();
};

#endif
