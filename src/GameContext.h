#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "Metadata.h"
#include "PlayerData.h"

#include "UsableId.h"

class ScriptEngine;
typedef class Usable Item;

class GameContext
{
   friend class GameState;
   ScriptEngine& m_scriptEngine;
   Metadata m_metadata;
   PlayerData m_playerData;

   ScriptEngine& getScriptEngine();
   Metadata& getMetadata();
   PlayerData& getCurrentPlayerData();

   public:
      GameContext(ScriptEngine& scriptEngine);
};

#endif

