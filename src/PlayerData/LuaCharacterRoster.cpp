/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "LuaCharacterRoster.h"
#include "CharacterRoster.h"

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int CharacterListL_AddToParty(lua_State* luaVM)
{
   bool success = false;
   const int nargs = lua_gettop(luaVM);
   CharacterRoster* characterRoster;

   if(nargs > 1)
   {
      characterRoster = luaW_to<CharacterRoster>(luaVM, 1);
      switch(nargs)
      {
         case 2:
         {
            characterRoster->addToParty(luaW_to<Character>(luaVM, 2));
            success = true;
            break;
         }
      }
   }

   lua_pushboolean(luaVM, success);
   return 1;
}

static luaL_reg characterRosterMetatable[] =
{
   { "add", CharacterListL_AddToParty },
   { NULL, NULL }
};

void luaopen_CharacterRoster(lua_State* luaVM)
{
   luaW_register<CharacterRoster>(luaVM, "CharacterRoster", NULL, characterRosterMetatable, NULL, NULL);
}
