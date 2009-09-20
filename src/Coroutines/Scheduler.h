#ifndef __SCRIPT_SCHEDULER_H_
#define __SCRIPT_SCHEDULER_H_

#include "TaskId.h"

#include <map>
#include <set>
#include <queue>

class Task;
class Thread;

/**
 * A scheduler for scheduling, resuming and blocking a set of Threads.
 * Each call to run resumes each ready thread, one by one, and retrieves a
 * return value indicating whether or not the Thread has yielded or completed.
 *
 * Objects that work with the threads can request that the scheduler block them
 * until completion of a task or until another Thread has completed execution.
 *
 * @author Noam Chitayat
 */
class Scheduler
{
   /** A list of threads. */
   typedef std::set<Thread*> ThreadList;

   /** A queue of threads. */
   typedef std::queue<Thread*> ThreadQueue;

   /** A list of blocked threads, indexed by the TaskId on which they are waiting */
   typedef std::map<TaskId, Thread*> BlockList;

   /** A list of joining/waiting threads, indexed by the Thread on which they are waiting */
   typedef std::map<Thread*, Thread*> JoinList;


   /** The list of currently blocked threads in this scheduler */
   BlockList blockedThreads;

   /** The list of currently joining/waiting threads in this scheduler */
   JoinList joiningThreads;

   /** The list of newly readied threads to be added to the ready list in the next run. */
   ThreadList unstartedThreads;

   /** The list of ready threads to be resumed during the next run. */
   ThreadList readyThreads;

   /** A list of threads to remove from the ready list after a run. */
   ThreadQueue finishedThreads;

   /** A list of threads to delete after a run. */
   ThreadQueue deletedThreads;

   /** The currently running thread */
   Thread* runningThread;

   /**
    * Signal that a Thread has run to completion so that waiting Threads
    * can be unblocked.
    *
    * @param thread The thread that has completed execution
    */
   void threadDone(Thread* thread);

   public:
      /**
       * Constructor. Initializes member variables.
       */
      Scheduler();

      /**
       * @return true iff there is a Thread currently running in the Scheduler
       */
      bool hasRunningThread();

      /**
       * Block a Thread on a specified instruction TicketId. Thread will be
       * readied again when the instruction is finished executing.
       *
       * @param task The task upon which the thread is waiting.
       *
       * @return a yield code from the Thread being blocked
       */
      int block(Task* task);

      /**
       * Add a Thread to the scheduler by enqueuing it to be started on the next run.
       *
       * @param thread The thread to start.
       */
      void start(Thread* thread);

      /**
       * Signals that an instruction has been completed so that the scheduler
       * can unblock any waiting Threads.
       *
       * @param finishedTask The ID of the instruction that has been finished.
       */
      void taskDone(TaskId finishedTask);

      /**
       * Block a Thread and make it wait until another Thread has finished
       * executing.
       *
       * @param joiningThread The Thread that will be waiting for the runningState to finish
       * @param runningThread The Thread on which the joining Thread is waiting.
       *
       * @return a yield code from the Thread being blocked
       */
      int join(Thread* runningThread);

      /**
       * Signal that a Thread has been finished and destroy the Thread.
       */
      void finished(Thread* thread);

      /**
       * Print the contents of the finished thread list to debug output.
       */
      void printFinishedQueue();

      /**
       * A scheduler run resumes each Thread in order and allows them to execute
       * until either completion or yielding.
       *
       * @param timePassed The amount of time that has passed since the last frame.
       *                   (roughly speaking, the amount of time since the last run)
       */
      void runThreads(long timePassed);
};

#endif
