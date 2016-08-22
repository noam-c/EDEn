/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef CANCELABLE_TASK_H
#define CANCELABLE_TASK_H

#include <atomic>
#include <future>
#include <type_traits>

/**
 * A wrapper for an std::async task. Exposes the ability
 * to cancel the task in the middle of execution.
 *
 * NOTE: CancelableTask relies on CO-OPERATIVE cancelation.
 * In other words, if the running task does not actually
 * check the cancel signal and exit early, then canceling
 * this task will just cause the current thread to block
 * until the task completes.
 *
 * @author Noam Chitayat
 */
template<typename Return> class CancelableTask final
{
   /** Cancel flag, used to signal a task to end ASAP. */
   std::atomic<bool> m_cancel;
   
   /** The future that will hold the result of the task. */
   std::shared_future<Return> m_future;

   public:
      /**
       * Launch an async task that can be signaled with a cancel flag.
       *
       * @param func The function to run asynchronously.
       * @param args The arguments to supply to the function.
       */
      template<typename Func, typename... Args> void runTask(Func&& func, Args&&... args)
      {
         // This assert just helps debug what may otherwise be a difficult-to-decipher
         // template complaint from the compiler.
         static_assert(
           std::is_same<decltype(func(args..., std::ref(m_cancel))), Return>::value,
           "CancelableTask<Return>: func does not return type Return for the given args.");

         // Signal the current task to cancel before the new one begins,
         // since reassigning m_future will cause the current thread to
         // block until the current task completes.
         cancel();

         m_cancel = false;
         m_future = std::async(
                      std::launch::async,
                      std::forward<Func>(func),
                      std::forward<Args>(args)...,
                      // Without std::ref, m_cancel is passed by copy to std::async, which
                      // doesn't work (can't copy atomics, also it's a template mismatch).
                      std::ref(m_cancel));
      }

      /**
       * Destructor.
       * Signals the task to cancel, and then blocks until it does so.
       */
      ~CancelableTask()
      {
         cancel();
      }

      /**
       * @return true iff there is a valid, running task
       */
      bool valid() const
      {
         return m_future.valid();
      }

      /**
       * Waits on the task completion for a specified duration.
       *
       * @return The result of waiting for the future (ready/timeout)
       */
      template <class Duration>
      std::future_status waitFor(Duration&& duration) const
      {
         return m_future.wait_for(std::forward<Duration>(duration));
      }

      /**
       * Gets the result of the assigned task. If the task
       * has not yet completed, then this function blocks
       * until the result is ready.
       *
       * @return the result of the assigned task.
       */
      const Return& get() const
      {
         return m_future.get();
      }
   
      /**
       * Gets the result of the assigned task. If the task
       * has not yet completed, then this function blocks
       * until the result is ready.
       *
       * @return the result of the assigned task.
       */
      Return& get()
      {
         return m_future.get();
      }
   
      /**
       * Signals the task to cancel, and then blocks until it does so.
       */
      void cancel()
      {
         m_cancel = true;
         if(m_future.valid())
         {
            m_future.wait();
         }
      }
};

#endif
