#ifndef __RESOURCE_H_
#define __RESOURCE_H_

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
{
   /**
    * True iff the resource has been successfully initialized.
    * False iff the resource is currently in an uninitialized stub/zombie state.
    */
   bool initialized;

   /**
    * Loading function for initialization of the resource from file.
    */
   virtual void load(const char* path) = 0;

   /**
    * The name of the resource.
    */
   ResourceKey name;

   public:
      /**
       * Constructor.
       */
      Resource(ResourceKey name);

      /**
       * Loads a resource from file and sets the resource to 'loaded' state
       * on success.
       */
      void initialize(const char* path);

      /**
       * @return true iff this resource has already been successfully loaded.
       */
      bool isInitialized();

      /**
       * @return the name of this Resource in string format.
       */
      std::string getResourceName();

      /**
       * @return the size that the resource takes up in memory.
       */
      virtual size_t getSize() = 0;
};

#endif
