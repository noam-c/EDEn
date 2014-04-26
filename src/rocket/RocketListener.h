#ifndef ROCKET_LISTENER_H
#define ROCKET_LISTENER_H

#include <Rocket/Core.h>
#include <functional>

class RocketListener : public Rocket::Core::EventListener
{
   Rocket::Core::Element* m_element;
   Rocket::Core::String m_eventType;
   bool m_capture;
   std::function<void(Rocket::Core::Event&)> m_processFunction;

   public:
      RocketListener(Rocket::Core::Element* element, Rocket::Core::String eventType, bool capture, std::function<void(Rocket::Core::Event&)> processFunction);
      
      RocketListener(const RocketListener& listener);

      RocketListener(RocketListener&& listener);
      
      ~RocketListener();

      void ProcessEvent(Rocket::Core::Event& event);
};

#endif
