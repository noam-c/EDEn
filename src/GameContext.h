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

typedef class Usable Item;
typedef unsigned int ItemId;
typedef unsigned int SkillId;
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
      Item* getItem(ItemId itemId) const;
      Skill* getSkill(SkillId skillId) const;

      void setCurrentPlayerData(PlayerData* data);
};

#endif

