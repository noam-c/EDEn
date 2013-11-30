#include "GameContext.h"
#include "GameState.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext() :
   m_scriptEngine(*this),
   m_metadata(*this),
   m_playerData(*this)
{
   Rocket::Core::Factory::RegisterEventListenerInstancer(new RocketScriptHandlerFactory(*this));
   m_scriptEngine.setPlayerData(&m_playerData);
}

GameContext::~GameContext()
{
}

ExecutionStack& GameContext::getExecutionStack()
{
   return m_executionStack;
}

ScriptEngine& GameContext::getScriptEngine()
{
   return m_scriptEngine;
}

PlayerData& GameContext::getCurrentPlayerData()
{
   return m_playerData;
}

Scheduler* GameContext::getCurrentScheduler() const
{
   GameState* currentState = m_executionStack.getCurrentState();
   if (currentState != NULL)
   {
      return currentState->getScheduler();
   }

   return NULL;
}

GameState::GameStateType GameContext::getCurrentStateType() const
{
   GameState* currentState = m_executionStack.getCurrentState();
   if (currentState != NULL)
   {
      return currentState->getStateType();
   }

   return GameState::UNKNOWN;
}

Item* GameContext::getItem(UsableId itemId) const
{
   return m_metadata.getItem(itemId);
}

Skill* GameContext::getSkill(UsableId skillId) const
{
   return m_metadata.getSkill(skillId);
}
