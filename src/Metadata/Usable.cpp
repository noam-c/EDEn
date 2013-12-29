/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Usable.h"
#include "json.h"

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
   m_id(node["id"].asInt()),
   m_name(node["name"].asString()),
   m_iconPath(node["icon"].asString()),
   m_usableScript(NULL)
{
}

Usable::~Usable()
{
   if(m_usableScript != NULL)
   {
      delete m_usableScript;
   }
}

UsableId Usable::getId() const
{
   return m_id;
}

const std::string& Usable::getName() const
{
   return m_name;
}

const std::string& Usable::getIconPath() const
{
   return m_iconPath;
}

void Usable::loadScript(ScriptEngine& scriptEngine)
{
   if(m_usableScript == NULL)
   {
      m_usableScript = createScript(scriptEngine);
   }
}

UsableScript* Usable::createScript(ScriptEngine& scriptEngine)
{
   return scriptEngine.createItemScript(*this);
}

bool Usable::use(ScriptEngine& scriptEngine, GameState::GameStateType gameStateType, Character* usingCharacter)
{
   loadScript(scriptEngine);

   switch(gameStateType)
   {
      case GameState::MENU:
         return m_usableScript->onMenuUse(usingCharacter);
      case GameState::FIELD:
         return m_usableScript->onFieldUse(usingCharacter);
      case GameState::BATTLE:
         return m_usableScript->onBattleUse(usingCharacter);
      default:
         // Fall through to return below.
         break;
   }

   DEBUG("Usable used in an unrecognized game state type. Usable script will not run.");
   return false;
}
