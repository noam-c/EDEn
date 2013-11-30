#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "ExecutionStack.h"
#include "ScriptEngine.h"
#include "Metadata.h"
#include "PlayerData.h"

/**
 * \todo Change this include to forward declarations when enums
 * can be forward declared in the product.
 */
#include "GameState.h"
#include "UsableId.h"

typedef class Usable Item;

class GameContext
{
   ExecutionStack m_executionStack;
   ScriptEngine m_scriptEngine;
   Metadata m_metadata;
   PlayerData m_playerData;

   public:
      GameContext();
      ~GameContext();

      ExecutionStack& getExecutionStack();
      ScriptEngine& getScriptEngine();
      PlayerData& getCurrentPlayerData();
      Scheduler* getCurrentScheduler() const;
      GameState::GameStateType getCurrentStateType() const;
      Item* getItem(UsableId itemId) const;
      Skill* getSkill(UsableId skillId) const;
};

#endif

