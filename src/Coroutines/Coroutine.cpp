/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Coroutine.h"
#include "CoroutineResults.h"

int Coroutine::nextCoroutineId = 0;

Coroutine::Coroutine() :
      m_coroutineId(nextCoroutineId++)
{
}

Coroutine::~Coroutine() = default;

void Coroutine::setResults(std::unique_ptr<ICoroutineResults>&& results)
{
   m_results.swap(results);
}

std::unique_ptr<ICoroutineResults>&& Coroutine::retrieveResults()
{
   return std::move(m_results);
}

int Coroutine::getId() const
{
   return m_coroutineId;
}

int Coroutine::yield(int numResults)
{
   return numResults;
}

