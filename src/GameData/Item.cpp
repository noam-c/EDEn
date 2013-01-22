/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Item.h"
#include "json.h"

#include "GameContext.h"
#include "ScriptEngine.h"
#include "ItemScript.h"

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

Item::Item(Json::Value& node) :
   id(node["id"].asInt()),
   name(node["name"].asString()),
   iconPath(node["icon"].asString()),
   itemScript(NULL)
{
}

Item::~Item()
{
   if(itemScript != NULL)
   {
      delete itemScript;
   }
}

const int Item::getId() const
{
   return id;
}

const std::string& Item::getName() const
{
   return name;
}

const std::string& Item::getIconPath() const
{
   return iconPath;
}

void Item::loadScript(GameContext& gameContext)
{
   if(itemScript == NULL)
   {
      itemScript = gameContext.getScriptEngine().createItemScript(*this);
   }
}

bool Item::use(GameContext& gameContext)
{
   loadScript(gameContext);

   switch(gameContext.getCurrentStateType())
   {
      case GameState::MENU:
         return itemScript->onMenuUse();
      case GameState::FIELD:
         return itemScript->onFieldUse();
      case GameState::BATTLE:
         return itemScript->onBattleUse();
   }

   DEBUG("Item used in an unrecognized game state type. Item script will not run.");
   return false;
}
