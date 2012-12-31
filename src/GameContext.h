#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "ExecutionStack.h"
#include "ScriptEngine.h"
#include "ItemData.h"

class Item;
class PlayerData;

class GameContext
{
   ExecutionStack executionStack;
   ScriptEngine scriptEngine;
   ItemData itemData;
   PlayerData* playerData;

   public:
      GameContext();
      ~GameContext();

      ExecutionStack& getExecutionStack();
      ScriptEngine& getScriptEngine();
      PlayerData* getCurrentPlayerData() const;
      Scheduler* getCurrentScheduler() const;
      Item* getItem(int itemId) const;

      void setCurrentPlayerData(PlayerData* data);
};

#endif

