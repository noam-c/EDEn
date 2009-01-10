#ifndef __RESOURCE_H_
#define __RESOURCE_H_

#include "ResourceException.h"

class Resource
{  public:
      virtual size_t getSize() = 0;
};

#endif
