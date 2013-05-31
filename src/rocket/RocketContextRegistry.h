/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_CONTEXT_REGISTRY_H
#define ROCKET_CONTEXT_REGISTRY_H

#include <Rocket/Core.h>
#include <Rocket/Core/Plugin.h>
#include <vector>

namespace Rocket
{
   namespace Core
   {
      class Context;
   };
};

/**
 * A registry used to track all active contexts, allowing
 * other components to perform operations on all contexts at once.
 *
 * @author Noam Chitayat
 */
class RocketContextRegistry : public Rocket::Core::Plugin
{
   /** The list of currently active Rocket contexts. */
   std::vector<Rocket::Core::Context*> activeRocketContexts;

   public:
      /**
       * Called when the plugin is registered to determine
       * which of the above event types the plugin is interested in.
       *
       * @return the event classes that the plugin is interested in.
       */
      virtual int GetEventClasses();

      /**
       * Called by Rocket when a new Rocket Context is created.
       *
       * @param context The context that was just created.
       */
      virtual void OnContextCreate(Rocket::Core::Context* context);

      /**
       * Called by Rocket when a Rocket Context is destroyed.
       *
       * @param context The context that is being destroyed.
       */
      virtual void OnContextDestroy(Rocket::Core::Context* context);

      /**
       * @return a list of all currently active contexts.
       */
      const std::vector<Rocket::Core::Context*>& getActiveContexts() const;
};

#endif
