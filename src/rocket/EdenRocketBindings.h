/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef EDEN_ROCKET_BINDINGS_H
#define EDEN_ROCKET_BINDINGS_H

#include "RocketListener.h"
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
template<typename T> class EdenRocketBindings
{
   /** The view model instance containing the event handler functions to call. */
   T* m_instance;

   /** The listeners created by the view model. */
   std::vector<RocketListener<T>* > m_listeners;

   /**
    * Private default constructor.
    */
   EdenRocketBindings() {};

   public:
      /**
       * Constructor.
       *
       * @param instance The view model that is constructing and
       *                 managing this EdenRocketBindings instance.
       */
      EdenRocketBindings(T* instance) : m_instance(instance) {}

      /**
       * Destructor.
       */
      ~EdenRocketBindings()
      {
         typename std::vector<RocketListener<T>* >::iterator iter;
         for(iter = m_listeners.begin(); iter != m_listeners.end(); ++iter)
         {
            delete *iter;
         }
      }

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
      void bindAction(Rocket::Core::ElementDocument* document, const char* id, const char* eventType, void (T::*handler)(Rocket::Core::Event*), bool capture = false)
      {
         Rocket::Core::Element* element = document->GetElementById(id);
         if(element != nullptr)
         {
            bindAction(element, eventType, handler);
         }
      }

      /**
       * Binds a listener for the specified event to the specified DOM element.
       *
       * @param element The DOM element to listen for events on.
       * @param eventType The event to listen for (e.g. "click", "keydown", etc.)
       * @param handler The function that will handle the event.
       * @param capture (Optional) True iff the event should be captured during the
       *                capturing phase instead of the bubbling phase.
       */
      void bindAction(Rocket::Core::Element* element, const char* eventType, void (T::*handler)(Rocket::Core::Event*), bool capture = false)
      {
         RocketListener<T>* listener = new RocketListener<T>(element, eventType, capture, std::bind1st(std::mem_fun(handler), m_instance));
         m_listeners.push_back(listener);
      }
};

#endif
