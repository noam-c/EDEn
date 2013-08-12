#ifndef ROCKET_LISTENER_H
#define ROCKET_LISTENER_H

#include <Rocket/Core.h>
#include <functional>

template<typename Handler> class RocketListener : public Rocket::Core::EventListener
{
   Rocket::Core::Element* m_element;
   Rocket::Core::String m_eventType;
   bool m_capture;
   typedef std::binder1st<std::mem_fun1_t<void, Handler, Rocket::Core::Event*> > ProcessFunctionType;
   ProcessFunctionType m_processFunction;

   public:
      RocketListener(Rocket::Core::Element* element, Rocket::Core::String eventType, bool capture, ProcessFunctionType processFunction)
         : m_element(element), m_eventType(eventType), m_capture(capture), m_processFunction(processFunction)
      {
         m_element->AddEventListener(eventType, this, capture);
      }

      ~RocketListener()
      {
         m_element->RemoveEventListener(m_eventType, this, m_capture);
      }

      void ProcessEvent(Rocket::Core::Event& event)
      {
         m_processFunction(&event);
      }
};

#endif
