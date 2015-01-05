/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef COROUTINE_H
#define COROUTINE_H

#include <memory>
#include <tuple>
#include <utility>

#include "CoroutineResults.h"

/**
 * A Coroutine is, in this case, an object that can yield, resume or block.
 * The typical scenario for a Coroutine object is a resumption (with the amount of
 * time since the last frame passed in), followed by blocking or by destruction.
 *
 * @author Noam Chitayat
 */
class Coroutine
{
   friend class Scheduler;

   /**
    * The next available coroutine ID to use for constructing a coroutine.
    */
   static int nextCoroutineId;

   /** A bag of result values from another task that this coroutine waited on. */
   std::unique_ptr<ICoroutineResults> m_results;

   /**
    * Assigns results to this coroutine.
    *
    * @param results A set of values resulting from the completion of a task dependency.
    */
   void setResults(std::unique_ptr<ICoroutineResults>&& results);

   protected:
      /** The numeric identified of this coroutine (currently just used for debugging) */
      int m_coroutineId;

      /**
       * @return the set of results that have been assigned to this coroutine
       * as a result of another task's execution.
       * @note This function will clear out the results from the Coroutine after completion.
       */
      std::unique_ptr<ICoroutineResults>&& retrieveResults();

   public:

      /**
       * Constructor. Initializes the coroutine ID.
       */
      Coroutine();

      /**
       * Destructor.
       */
      virtual ~Coroutine() = default;

      /**
       * @return the numeric identifier for this Coroutine.
       */
      int getId() const;

      /**
       * Resume this Coroutine, or run through its logic.
       *
       * @param timePassed The amount of time that has passed since the last frame
       *                   (roughly speaking, since the last run of this Coroutine)
       *
       * @return true iff the Coroutine has run to completion, false if it has
       *              yielded or encountered an error.
       */
      virtual bool resume(long timePassed) = 0;

      /**
       * Yield the currently running coroutine, if it is a coroutine.
       * Otherwise, do nothing.
       *
       * @return a yield code, indicating a yield to a calling class, or 0 if
       *         this coroutine is not a coroutine.
       */
      virtual int yield(int numResults);
};

#endif
