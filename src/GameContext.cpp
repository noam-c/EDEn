#include "GameContext.h"
#include "ScriptEngine.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext(ScriptEngine& scriptEngine) :
   m_scriptEngine(scriptEngine),
   m_metadata(*this),
   m_playerData(m_metadata)
{
   Rocket::Core::Factory::RegisterEventListenerInstancer(new RocketScriptHandlerFactory(m_scriptEngine));
   m_scriptEngine.setPlayerData(&m_playerData);
}

GameContext::~GameContext()
{
}

const Metadata& GameContext::getMetadata() const
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
