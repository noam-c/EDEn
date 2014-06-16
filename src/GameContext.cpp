#include "GameContext.h"
#include "ScriptEngine.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext(ScriptEngine& scriptEngine) :
   m_scriptEngine(scriptEngine),
   m_metadata(m_scriptEngine)
{
   Rocket::Core::Factory::RegisterEventListenerInstancer(new RocketScriptHandlerFactory(m_scriptEngine));
}

Metadata& GameContext::getMetadata()
{
   return m_metadata;
}

ScriptEngine& GameContext::getScriptEngine()
{
   return m_scriptEngine;
}
