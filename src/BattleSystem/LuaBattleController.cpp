/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "LuaBattleController.h"

#include "BattleController.h"
#include "EnumUtils.h"
#include "Direction.h"
#include "Point2D.h"
#include "ScriptUtilities.h"
#include "Size.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_SCRIPT_ENG

static int BattleControllerL_AddCombatant(lua_State* luaVM)
{
   BattleController* battleController = luaW_check<BattleController>(luaVM, 1);
   if (battleController == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string combatantId;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "id", combatantId))
   {
      return lua_error(luaVM);
   }

   std::string spritesheetName;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "spritesheet", spritesheetName))
   {
      return lua_error(luaVM);
   }

   int x;
   if(!ScriptUtilities::getParameter(luaVM, 2, 3, "x", x))
   {
      return lua_error(luaVM);
   }

   int y;
   if(!ScriptUtilities::getParameter(luaVM, 2, 4, "y", y))
   {
      return lua_error(luaVM);
   }

   int width;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "width", width))
   {
      width = 32;
   }

   int height;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "height", height))
   {
      height = 32;
   }

   int directionValue;
   if(!ScriptUtilities::getParameter(luaVM, 2, -1, "direction", directionValue))
   {
      directionValue = EnumUtils::toNumber(geometry::Direction::DOWN);
   }

   DEBUG("Adding NPC %s with spritesheet %s", combatantId.c_str(), spritesheetName.c_str());
   DEBUG("NPC Location will be (%d, %d)", x, y);

   const geometry::Point2D actorLocation(x, y);
   const geometry::Size actorSize(width, height);
   const geometry::Direction direction = static_cast<geometry::Direction>(directionValue);

   auto combatActor = battleController->addCombatant(combatantId, actorLocation, actorSize, direction, spritesheetName);

   luaW_push<CombatActor>(luaVM, combatActor);
   return 1;
}

static luaL_Reg battleControllerMetatable[] =
{
   { "addCombatant", BattleControllerL_AddCombatant },
   { nullptr, nullptr }
};

void luaopen_BattleController(lua_State* luaVM)
{
   luaW_register<BattleController>(luaVM, "BattleController", nullptr, battleControllerMetatable, nullptr, nullptr);
}
