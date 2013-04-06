/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "LuaPlayerCharacter.h"
#include "ScriptUtilities.h"
#include "PlayerCharacter.h"
#include "Point2D.h"

static int PlayerCharacterL_Show(lua_State* luaVM)
{
   PlayerCharacter* playerCharacter = luaW_check<PlayerCharacter>(luaVM, 1);
   if (playerCharacter == NULL)
   {
      return lua_error(luaVM);
   }

   int x;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "x", x))
   {
      return lua_error(luaVM);
   }

   int y;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "y", y))
   {
      return lua_error(luaVM);
   }

   playerCharacter->addToMap(shapes::Point2D(x, y));

   return 0;
}

static int PlayerCharacterL_Remove(lua_State* luaVM)
{
   PlayerCharacter* playerCharacter = luaW_check<PlayerCharacter>(luaVM, 1);
   if (playerCharacter == NULL)
   {
      return lua_error(luaVM);
   }

   playerCharacter->removeFromMap();

   return 0;
}

static int PlayerCharacterL_SetLocation(lua_State* luaVM)
{
   PlayerCharacter* playerCharacter = luaW_check<PlayerCharacter>(luaVM, 1);
   if (playerCharacter == NULL)
   {
      return lua_error(luaVM);
   }

   int x;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "x", x))
   {
      return lua_error(luaVM);
   }

   int y;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "y", y))
   {
      return lua_error(luaVM);
   }

   playerCharacter->setLocation(shapes::Point2D(x, y));

   return 0;
}

static luaL_reg playerCharacterMetatable[] =
{
   { "show", PlayerCharacterL_Show },
   { "remove", PlayerCharacterL_Remove },
   { "setLocation", PlayerCharacterL_SetLocation },
   { NULL, NULL }
};

void luaopen_PlayerCharacter(lua_State* luaVM)
{
   luaW_register<PlayerCharacter>(luaVM, "PlayerCharacter", NULL, playerCharacterMetatable, NULL, NULL);
   luaW_extend<PlayerCharacter, Actor>(luaVM);
}
