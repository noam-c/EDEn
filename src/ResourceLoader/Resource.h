/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef RESOURCE_H
#define RESOURCE_H

#include "ResourceKey.h"

/**
 * A Resource is any object that is loaded from a file resource and managed
 * by the ResourceLoader in order to reduce loads from file.
 *
 * Anything that needs to load information from file should be considered as
 * a candidate for becoming a Resource, in order to better cache data.
 *
 * Any subclass of Resource needs to implement a basic constructor, but must
 * delegate the actual resource load and initialization to the load function.
 * By doing this, we can contain load failures without screwing up the actual
 * construction of the object in memory (thus, we can have more control over
 * how the resource is freed.
 * Any resource that fails to load properly (due to file not found or
 * corrupt/malformed data falls back on a "zombie resource" state where its
 * data is uninitialized. The rest of the code can reference it SAFELY (this is
 * the key!), but it will simply do nothing if it was not initialized.
 * The ResourceLoader may attempt to reload the Resource from time to time if
 * it is in a zombie state.
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
    * The name of the resource.
    */
   const ResourceKey& name;

   /**
    * Loading function for initialization of the resource from file.
    *
    * @param path The file path where the resource is located.
    */
   virtual void load(const std::string& path) = 0;

   public:
      /**
       * Constructor.
       *
       * @param name The unique name of this resource.
       */
      Resource(const ResourceKey& name);

      /**
       * Loads a resource from file and sets the resource to 'loaded' state
       * on success.
       *
       * @param path The file path where the resource is located.
       */
      void initialize(const char* path);

      /**
       * @return true iff this resource has already been successfully loaded.
       */
      bool isInitialized() const;

      /**
       * @return the name of this Resource in string format.
       */
      std::string getResourceName() const;

      /**
       * @return the size that the resource takes up in memory.
       */
      virtual size_t getResourceSize() const = 0;

      /**
       * Abstract destructor (forces all Resources to have destructors).
       */
      virtual ~Resource() = 0;
};

#endif
