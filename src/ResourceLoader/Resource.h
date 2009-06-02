#ifndef __RESOURCE_H_
#define __RESOURCE_H_

#include "ResourceException.h"
#include "ResourceKey.h"

class Resource
{  /**
    * \todo Document.
    */
   ResourceKey name;
   
   public:
      /**
       * \todo Document.
       */
      Resource(ResourceKey name);

      /**
       * \todo Document.
       */
      std::string getName();

      /**
       * @return the size that the resource takes up in memory.
       */
      virtual size_t getSize() = 0;
};

#endif
