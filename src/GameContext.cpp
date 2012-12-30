#include "GameContext.h"
#include "GameState.h"

#include <Rocket/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext() : scriptEngine(*this)
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

PlayerData* GameContext::getCurrentPlayerData()
{
   return playerData;
}

Scheduler* GameContext::getCurrentScheduler()
{
   GameState* currentState = executionStack.getCurrentState();
   if (currentState != NULL)
   {
      return currentState->getScheduler();
   }

   return NULL;
}

void GameContext::setCurrentPlayerData(PlayerData* data)
{
   playerData = data;
}
