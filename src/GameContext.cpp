#include "GameContext.h"
#include "ScriptEngine.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext(ScriptEngine& scriptEngine) :
   m_scriptEngine(scriptEngine),
   m_metadata(m_scriptEngine),
   m_playerData(m_metadata)
{
   Rocket::Core::Factory::RegisterEventListenerInstancer(new RocketScriptHandlerFactory(m_scriptEngine));
   m_scriptEngine.setPlayerData(&m_playerData);
}

GameContext::~GameContext()
{
}

Metadata& GameContext::getMetadata()
{
   return m_metadata;
}

ScriptEngine& GameContext::getScriptEngine()
{
   return m_scriptEngine;
}

PlayerData& GameContext::getCurrentPlayerData()
{
   return m_playerData;
}
