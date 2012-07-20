/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Thread.h"

int Thread::nextThreadId = 0;

Thread::Thread()
{
   threadId = nextThreadId++;
}

int Thread::getId() const
{
   return threadId;
}

int Thread::yield()
{
   return 0;
}

Thread::~Thread()
{
}
