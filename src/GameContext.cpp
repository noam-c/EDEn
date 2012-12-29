#include "GameContext.h"
#include "GameState.h"

GameContext::GameContext() : scriptEngine(*this)
{
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
