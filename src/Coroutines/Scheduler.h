/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef SCRIPT_SCHEDULER_H
#define SCRIPT_SCHEDULER_H

#include "TaskId.h"

#include <map>
#include <set>
#include <queue>

class Task;
class Coroutine;

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
   typedef std::set<Coroutine*> CoroutineList;

   /** A queue of coroutines. */
   typedef std::queue<Coroutine*> CoroutineQueue;

   /** A list of blocked coroutines, indexed by the TaskId on which they are waiting */
   typedef std::map<TaskId, Coroutine*> BlockList;

   /** A list of joining/waiting coroutines, indexed by the Coroutine on which they are waiting */
   typedef std::map<Coroutine*, Coroutine*> JoinList;


   /** The list of currently blocked coroutines in this scheduler */
   BlockList blockedCoroutines;

   /** The list of currently joining/waiting coroutines in this scheduler */
   JoinList joiningCoroutines;

   /** The list of newly readied coroutines to be added to the ready list in the next run. */
   CoroutineList unstartedCoroutines;

   /** The list of ready coroutines to be resumed during the next run. */
   CoroutineList readyCoroutines;

   /** A list of coroutines to remove from the ready list after a run. */
   CoroutineQueue finishedCoroutines;

   /** A list of coroutines to delete after a run. */
   CoroutineQueue deletedCoroutines;

   /** The currently running coroutine */
   Coroutine* runningCoroutine;

   /**
    * Signal that a Coroutine has run to completion so that waiting Coroutines
    * can be unblocked.
    *
    * @param coroutine The coroutine that has completed execution
    */
   void coroutineDone(Coroutine* coroutine);

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
      int block(Task* task);

      /**
       * Add a Coroutine to the scheduler by enqueuing it to be started on the next run.
       *
       * @param coroutine The coroutine to start.
       */
      void start(Coroutine* coroutine);

      /**
       * Signals that an instruction has been completed so that the scheduler
       * can unblock any waiting Coroutines.
       *
       * @param finishedTask The ID of the instruction that has been finished.
       */
      void taskDone(TaskId finishedTask);

      /**
       * Block a Coroutine and make it wait until another Coroutine has finished
       * executing.
       *
       * @param joiningCoroutine The Coroutine that will be waiting for the runningState to finish
       * @param runningCoroutine The Coroutine on which the joining Coroutine is waiting.
       *
       * @return a yield code from the Coroutine being blocked
       */
      int join(Coroutine* runningCoroutine);

      /**
       * Signal that a Coroutine has been finished and destroy the Coroutine.
       */
      void finished(Coroutine* coroutine);

      /**
       * Print the contents of the finished coroutine list to debug output.
       */
      void printFinishedQueue();

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
