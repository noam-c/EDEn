#include "Thread.h"

int Thread::nextThreadId = 0;

Thread::Thread()
{
   threadId = nextThreadId++;
}

int Thread::getId()
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
