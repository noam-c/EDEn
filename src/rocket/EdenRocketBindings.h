#ifndef EDEN_ROCKET_BINDINGS_H
#define EDEN_ROCKET_BINDINGS_H

#include "RocketListener.h"
#include <vector>

namespace Rocket
{
   namespace Core
   {
      class Element;
      class ElementDocument;
      class Event;
   };
};

template<typename T> class EdenRocketBindings
{
   T* instance;
   std::vector<RocketListener<T>* > listeners;

   EdenRocketBindings() {};

   public:
      EdenRocketBindings(T* instance) : instance(instance) {}

      ~EdenRocketBindings()
      {
         typename std::vector<RocketListener<T>* >::iterator iter;
         for(iter = listeners.begin(); iter != listeners.end(); ++iter)
         {
            delete *iter;
         }
      }

      void bindAction(Rocket::Core::ElementDocument* document, const char* id, const char* eventType, void (T::*function)(Rocket::Core::Event*), bool capture = false)
      {
         Rocket::Core::Element* element = document->GetElementById(id);
         if(element != NULL)
         {
            bindAction(element, eventType, function);
         }
      }

      void bindAction(Rocket::Core::Element* element, const char* eventType, void (T::*function)(Rocket::Core::Event*), bool capture = false)
      {
         RocketListener<T>* listener = new RocketListener<T>(element, eventType, capture, std::bind1st(std::mem_fun(function), instance));
         listeners.push_back(listener);
      }
};

#endif
