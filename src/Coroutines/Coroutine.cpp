/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Coroutine.h"

int Coroutine::nextCoroutineId = 0;

Coroutine::Coroutine() :
      m_coroutineId(nextCoroutineId++)
{
}

Coroutine::~Coroutine()
{
}

int Coroutine::getId() const
{
   return m_coroutineId;
}

int Coroutine::yield()
{
   return 0;
}
