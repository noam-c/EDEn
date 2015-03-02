/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef COROUTINE_RESULTS_H
#define COROUTINE_RESULTS_H

#include <initializer_list>
#include <memory>
#include <tuple>
#include <utility>

#include "IntegerSequence.h"

/**
 * An operation that can be used on a set of coroutine results.
 *
 * @author Noam Chitayat
 */
class ICoroutineResultOp
{
   public:
      virtual void operator()(bool arg) = 0;
      virtual void operator()(int arg) = 0;
      virtual ~ICoroutineResultOp() = default;
};

/**
 * A set of return values supplied to a coroutine after a Task that
 * it is blocked on completes.
 * Can contain any type of parameter as long as
 * given types are supported in <code>ICoroutineResultOp</code>
 * (so add new types if they are not yet supported).
 *
 * @author Noam Chitayat
 */
class ICoroutineResults
{
   public:
      virtual void forEach(ICoroutineResultOp& op) = 0;
      virtual size_t getSize() const = 0;
      virtual ~ICoroutineResults() = default;
};

/**
 * A variadic set of results that can be used to construct result sets of any shape.
 *
 * @author Noam Chitayat
 */
template<typename ... Results> class CoroutineResults final : public ICoroutineResults
{
   std::tuple<Results...> m_results;

   public:
      CoroutineResults(Results&& ... results) :
         m_results(std::forward_as_tuple(results...))
      {
      }

      size_t getSize() const override
      {
         return sizeof...(Results);
      }

      template<int... Indices> inline void operateOnResults(ICoroutineResultOp& op, IntegerSequence<Indices...>)
      {
         // The initializer list here is never used, but the creation of the list
         // expands out to an iteration across the entire tuple in order, which is what we want.
         // Shorthand for compile time generation of something like, "for (index : Indices) { op(std::get<index>(m_results); }".
         std::initializer_list<int>({
            (
               op(std::get<Indices>(m_results)), 0 // end expression with a throwaway result value since "op" has no return value
            )...
         });
      }

      void forEach(ICoroutineResultOp& op) override
      {
         // Since std::tuples don't support runtime iteration,
         // iterate over (and operate on) the results tuple using a compile-time integer sequence.
         operateOnResults(op, GenerateIntegerSequence<sizeof...(Results)>());
      }

      ~CoroutineResults() override = default;
};

#endif
