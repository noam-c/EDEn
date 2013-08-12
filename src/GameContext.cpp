#include "GameContext.h"
#include "GameState.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext() :
   m_scriptEngine(*this),
   m_metadata(*this)
{
   Rocket::Core::Factory::RegisterEventListenerInstancer(new RocketScriptHandlerFactory(*this));
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

PlayerData* GameContext::getCurrentPlayerData() const
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

void GameContext::setCurrentPlayerData(PlayerData* playerData)
{
   m_playerData = playerData;
}
