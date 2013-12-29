#include "GameContext.h"
#include "ScriptEngine.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext(ScriptEngine& scriptEngine) :
   m_scriptEngine(scriptEngine),
   m_metadata(*this),
   m_playerData(*this)
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

Item* GameContext::getItem(UsableId itemId) const
{
   return m_metadata.getItem(itemId);
}

Skill* GameContext::getSkill(UsableId skillId) const
{
   return m_metadata.getSkill(skillId);
}
