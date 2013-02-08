#include "GameContext.h"
#include "GameState.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext() :
   scriptEngine(*this),
   metadata(*this)
{
   Rocket::Core::Factory::RegisterEventListenerInstancer(new RocketScriptHandlerFactory(*this));
}

GameContext::~GameContext()
{
}

ExecutionStack& GameContext::getExecutionStack()
{
   return executionStack;
}

ScriptEngine& GameContext::getScriptEngine()
{
   return scriptEngine;
}

PlayerData* GameContext::getCurrentPlayerData() const
{
   return playerData;
}

Scheduler* GameContext::getCurrentScheduler() const
{
   GameState* currentState = executionStack.getCurrentState();
   if (currentState != NULL)
   {
      return currentState->getScheduler();
   }

   return NULL;
}

GameState::GameStateType GameContext::getCurrentStateType() const
{
   GameState* currentState = executionStack.getCurrentState();
   if (currentState != NULL)
   {
      return currentState->getStateType();
   }

   return GameState::UNKNOWN;
}

Item* GameContext::getItem(ItemId itemId) const
{
   return metadata.getItem(itemId);
}

Skill* GameContext::getSkill(SkillId skillId) const
{
   return metadata.getSkill(skillId);
}

void GameContext::setCurrentPlayerData(PlayerData* data)
{
   playerData = data;
}
