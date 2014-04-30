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

const char* Usable::ID_ATTRIBUTE = "id";
const char* Usable::NAME_ATTRIBUTE = "name";
const char* Usable::ICON_ATTRIBUTE = "icon";

Usable::Usable(Json::Value& node) :
   m_id(node[Usable::ID_ATTRIBUTE].asInt()),
   m_name(node[Usable::NAME_ATTRIBUTE].asString()),
   m_iconPath(node[Usable::ICON_ATTRIBUTE].asString()),
   m_usableScript(nullptr)
{
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
   if(!m_usableScript)
   {
      m_usableScript = std::move(createScript(scriptEngine));
   }
}

std::shared_ptr<UsableScript> Usable::createScript(ScriptEngine& scriptEngine)
{
   return scriptEngine.createItemScript(*this);
}

bool Usable::use(ScriptEngine& scriptEngine, GameStateType gameStateType, Character* usingCharacter)
{
   loadScript(scriptEngine);

   switch(gameStateType)
   {
      case GameStateType::MENU:
         return m_usableScript->onMenuUse(usingCharacter);
      case GameStateType::FIELD:
         return m_usableScript->onFieldUse(usingCharacter);
      case GameStateType::BATTLE:
         return m_usableScript->onBattleUse(usingCharacter);
      default:
         // Fall through to return below.
         break;
   }

   DEBUG("Usable used in an unrecognized game state type. Usable script will not run.");
   return false;
}
