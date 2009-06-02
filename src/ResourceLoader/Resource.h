#ifndef __RESOURCE_H_
#define __RESOURCE_H_

#include "ResourceException.h"
#include "ResourceKey.h"

/**
 * A Resource is any object that is loaded from a file resource and managed
 * by the ResourceLoader in order to reduce loads from file.
 *
 * Anything that needs to load information from file should be considered as
 * a candidate for becoming a Resource, in order to better cache data.
 *
 * @author Noam Chitayat
 */
class Resource
{  /**
    * The name of the resource.
    */
   ResourceKey name;
   
   public:
      /**
       * Constructor.
       */
      Resource(ResourceKey name);

      /**
       * @return the name of this Resource in string format.
       */
      std::string getName();

      /**
       * @return the size that the resource takes up in memory.
       */
      virtual size_t getSize() = 0;
};

#endif
