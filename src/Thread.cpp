#include "Thread.h"

int Thread::numThreads = 0;

int Thread::getNumThreads()
{  return numThreads;
}

Thread::Thread()
{  threadId = numThreads++;
}

int Thread::getId()
{  return threadId;
}
