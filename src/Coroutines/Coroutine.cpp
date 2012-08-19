/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Coroutine.h"

int Coroutine::nextCoroutineId = 0;

Coroutine::Coroutine()
{
   coroutineId = nextCoroutineId++;
}

int Coroutine::getId() const
{
   return coroutineId;
}

int Coroutine::yield()
{
   return 0;
}

Coroutine::~Coroutine()
{
}
