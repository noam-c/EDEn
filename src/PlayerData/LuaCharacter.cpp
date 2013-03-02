/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "LuaCharacter.h"
#include "Character.h"

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int CharacterL_GetName(lua_State* luaVM)
{
   std::string name;
   const int nargs = lua_gettop(luaVM);
   switch(nargs)
   {
      case 1:
      {
         const Character* character = luaW_to<Character>(luaVM, 1);
         if(character != NULL)
         {
            name = character->getName();
         }
         break;
      }
   }

   lua_pushstring(luaVM, name.c_str());
   return 1;
}

static int CharacterL_AddSkill(lua_State* luaVM)
{
   bool success = false;
   const int nargs = lua_gettop(luaVM);
   Character* character;

   if(nargs > 1)
   {
      character = luaW_to<Character>(luaVM, 1);
      switch(nargs)
      {
         case 2:
         {
            success = character->addSkill(lua_tonumber(luaVM, 2));
            break;
         }
      }
   }

   lua_pushboolean(luaVM, success);
   return 1;
}

static luaL_reg characterMetatable[] =
{
   { "getName", CharacterL_GetName },
   { "addSkill", CharacterL_AddSkill },
   { NULL, NULL }
};

void luaopen_Character(lua_State* luaVM)
{
   luaW_register<Character>(luaVM, "Character", NULL, characterMetatable, NULL, NULL);
}
