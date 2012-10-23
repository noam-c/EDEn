#ifndef ROCKET_LISTENER_H
#define ROCKET_LISTENER_H

#include <Rocket/Core.h>
#include <functional>

template<typename Handler> class RocketListener : public Rocket::Core::EventListener
{
   Rocket::Core::Element* element;
   Rocket::Core::String eventType;
   bool capture;
   typedef std::binder1st<std::mem_fun1_t<void, Handler, Rocket::Core::Event*> > ProcessFunctionType;
   ProcessFunctionType processFunction;

   public:
      RocketListener(Rocket::Core::Element* element, Rocket::Core::String eventType, bool capture, ProcessFunctionType processFunction)
         : element(element), eventType(eventType), capture(capture), processFunction(processFunction)
      {
         element->AddEventListener(eventType, this, capture);
      }

      ~RocketListener()
      {
         element->RemoveEventListener(eventType, this, capture);
      }

      void ProcessEvent(Rocket::Core::Event& event)
      {
         processFunction(&event);
      }
};

#endif
