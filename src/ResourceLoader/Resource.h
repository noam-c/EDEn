#ifndef __RESOURCE_H_
#define __RESOURCE_H_

#include "ResourceException.h"

class Resource
{  public:
      /**
       * @return the size that the resource takes up in memory.
       */
      virtual size_t getSize() = 0;
};

#endif
