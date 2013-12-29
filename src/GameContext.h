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
   ScriptEngine& m_scriptEngine;
   Metadata m_metadata;
   PlayerData m_playerData;

   public:
      GameContext(ScriptEngine& scriptEngine);
      ~GameContext();

      ScriptEngine& getScriptEngine();
      const Metadata& getMetadata() const;
      PlayerData& getCurrentPlayerData();
      Item* getItem(UsableId itemId) const;
      Skill* getSkill(UsableId skillId) const;
};

#endif

