/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef EDEN_ROCKET_BINDINGS_H
#define EDEN_ROCKET_BINDINGS_H

#include "RocketListener.h"
#include <functional>
#include <memory>
#include <vector>

/**
 * Manages a group of RML event listeners bound to a view model T.
 * To use this, create an instance of EdenRocketBindings in the view model
 * and bind your event listeners using the bindAction function.
 * When this class is cleaned up, all the created listeners will also be
 * removed from the RML document.
 *
 * @author Noam Chitayat
 */
class EdenRocketBindings
{
   /** The listeners created by the view model. */
   std::vector<RocketListener> m_listeners;

   public:
      /**
       * Constructor.
       */
      EdenRocketBindings();

      /**
       * Destructor.
       */
      ~EdenRocketBindings();

      /**
       * Binds a listener for the specified event to a DOM element
       * (specified by id) in the document.
       *
       * @param document The RML document containing the element to listen for events on.
       * @param id The id of the DOM element to listen for events on.
       * @param eventType The event to listen for (e.g. "click", "keydown", etc.)
       * @param handler The function that will handle the event.
       * @param capture (Optional) True iff the event should be captured during the
       *                capturing phase instead of the bubbling phase.
       */
      void bindAction(Rocket::Core::ElementDocument* document, const char* id, const char* eventType, std::function<void(Rocket::Core::Event&)> handler, bool capture = false);

      /**
       * Binds a listener for the specified event to the specified DOM element.
       *
       * @param element The DOM element to listen for events on.
       * @param eventType The event to listen for (e.g. "click", "keydown", etc.)
       * @param handler The function that will handle the event.
       * @param capture (Optional) True iff the event should be captured during the
       *                capturing phase instead of the bubbling phase.
       */
      void bindAction(Rocket::Core::Element* element, const char* eventType, std::function<void(Rocket::Core::Event&)> handler, bool capture = false);
};

#endif
