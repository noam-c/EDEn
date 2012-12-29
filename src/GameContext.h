#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "ExecutionStack.h"
#include "ScriptEngine.h"

class PlayerData;

class GameContext
{
   ExecutionStack executionStack;
   ScriptEngine scriptEngine;
   PlayerData* playerData;

   public:
      GameContext();
      ~GameContext();

      ExecutionStack& getExecutionStack();
      ScriptEngine& getScriptEngine();
      PlayerData* getCurrentPlayerData();
      Scheduler* getCurrentScheduler();

      void setCurrentPlayerData(PlayerData* data);
};

#endif

