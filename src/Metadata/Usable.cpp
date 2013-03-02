/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Usable.h"
#include "json.h"

#include "GameContext.h"
#include "ScriptEngine.h"
#include "UsableScript.h"

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

#include "DebugUtils.h"
const int debugFlag = DEBUG_METADATA;

Usable::Usable(Json::Value& node) :
   id(node["id"].asInt()),
   name(node["name"].asString()),
   iconPath(node["icon"].asString()),
   usableScript(NULL)
{
}

Usable::~Usable()
{
   if(usableScript != NULL)
   {
      delete usableScript;
   }
}

UsableId Usable::getId() const
{
   return id;
}

const std::string& Usable::getName() const
{
   return name;
}

const std::string& Usable::getIconPath() const
{
   return iconPath;
}

void Usable::loadScript(GameContext& gameContext)
{
   if(usableScript == NULL)
   {
      usableScript = createScript(gameContext);
   }
}

UsableScript* Usable::createScript(GameContext& gameContext)
{
   return gameContext.getScriptEngine().createItemScript(*this);
}

bool Usable::use(GameContext& gameContext, Character* usingCharacter)
{
   loadScript(gameContext);

   switch(gameContext.getCurrentStateType())
   {
      case GameState::MENU:
         return usableScript->onMenuUse(usingCharacter);
      case GameState::FIELD:
         return usableScript->onFieldUse(usingCharacter);
      case GameState::BATTLE:
         return usableScript->onBattleUse(usingCharacter);
   }

   DEBUG("Usable used in an unrecognized game state type. Usable script will not run.");
   return false;
}
