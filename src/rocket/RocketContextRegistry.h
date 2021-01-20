/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_CONTEXT_REGISTRY_H
#define ROCKET_CONTEXT_REGISTRY_H

#include <RmlUi/Core.h>
#include <RmlUi/Core/Plugin.h>
#include <vector>

namespace Rml
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
class RocketContextRegistry final : public Rml::Core::Plugin
{
   /** The list of currently active Rocket contexts. */
   std::vector<Rml::Core::Context*> m_activeRocketContexts;

   public:
      /**
       * Called when the plugin is registered to determine
       * which of the above event types the plugin is interested in.
       *
       * @return the event classes that the plugin is interested in.
       */
      int GetEventClasses() override;

      /**
       * Called by Rocket when a new Rocket Context is created.
       *
       * @param context The context that was just created.
       */
      void OnContextCreate(Rml::Core::Context* context) override;

      /**
       * Called by Rocket when a Rocket Context is destroyed.
       *
       * @param context The context that is being destroyed.
       */
      void OnContextDestroy(Rml::Core::Context* context) override;

      /**
       * @return a list of all currently active contexts.
       */
      const std::vector<Rml::Core::Context*>& getActiveContexts() const;
};

#endif
