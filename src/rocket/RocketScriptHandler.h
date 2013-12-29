#ifndef ROCKET_SCRIPT_HANDLER_H
#define ROCKET_SCRIPT_HANDLER_H

#include <Rocket/Core.h>

class ScriptEngine;

class RocketScriptHandler : public Rocket::Core::EventListener
{
   ScriptEngine& m_scriptEngine;
   const std::string m_scriptString;

   public:
      RocketScriptHandler(ScriptEngine& scriptEngine, Rocket::Core::String scriptString);

      virtual ~RocketScriptHandler();

      virtual void ProcessEvent(Rocket::Core::Event& event);

      virtual void OnDetach(Rocket::Core::Element* element);
};

#endif
