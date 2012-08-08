#ifndef ROCKET_LISTENER_H
#define ROCKET_LISTENER_H

#include <Rocket/Core.h>
#include <functional>

template<typename Handler> class RocketListener : public Rocket::Core::EventListener
{
   typedef std::binder1st<std::mem_fun1_t<void, Handler, Rocket::Core::Event*> > ProcessFunctionType;
   ProcessFunctionType processFunction;

   public:
      RocketListener(ProcessFunctionType processFunction)
         : processFunction(processFunction)
      {}

      void ProcessEvent(Rocket::Core::Event& event)
      {
         processFunction(&event);
      }
};

#endif
