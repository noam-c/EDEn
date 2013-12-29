#ifndef ROCKET_SCRIPT_HANDLER_FACTORY_H
#define ROCKET_SCRIPT_HANDLER_FACTORY_H

#include <Rocket/Core.h>

class ScriptEngine;
class RocketScriptHandler;

class RocketScriptHandlerFactory : public Rocket::Core::EventListenerInstancer
{
   ScriptEngine& m_scriptEngine;

   public:
      RocketScriptHandlerFactory(ScriptEngine& scriptEngine);
      virtual ~RocketScriptHandlerFactory();

      virtual Rocket::Core::EventListener* InstanceEventListener(
            const Rocket::Core::String& value, Rocket::Core::Element* element);

      virtual void Release();
};

#endif
