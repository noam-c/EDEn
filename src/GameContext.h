#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "ExecutionStack.h"
#include "ScriptEngine.h"
#include "Metadata.h"

/**
 * \todo Change this include to forward declarations when enums
 * can be forward declared in the product.
 */
#include "GameState.h"
#include "UsableId.h"

typedef class Usable Item;
class PlayerData;

class GameContext
{
   ExecutionStack executionStack;
   ScriptEngine scriptEngine;
   Metadata metadata;
  
   /**
    *  \todo Load or set PlayerData in GameContext and, where appropriate,
    *  pass the context around instead of the PlayerData.
    */
   PlayerData* playerData;

   public:
      GameContext();
      ~GameContext();

      ExecutionStack& getExecutionStack();
      ScriptEngine& getScriptEngine();
      PlayerData* getCurrentPlayerData() const;
      Scheduler* getCurrentScheduler() const;
      GameState::GameStateType getCurrentStateType() const;
      Item* getItem(UsableId itemId) const;
      Skill* getSkill(UsableId skillId) const;

      void setCurrentPlayerData(PlayerData* data);
};

#endif

