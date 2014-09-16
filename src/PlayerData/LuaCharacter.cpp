/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "LuaCharacter.h"
#include "Character.h"

#include "ScriptUtilities.h"

static int CharacterL_GetName(lua_State* luaVM)
{
   const Character* character = luaW_check<Character>(luaVM, 1);
   if(character == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string name = character->getName();
   lua_pushstring(luaVM, name.c_str());
   return 1;
}

static luaL_Reg characterMetatable[] =
{
   { "getName", CharacterL_GetName },
   { nullptr, nullptr }
};

void luaopen_Character(lua_State* luaVM)
{
   luaW_register<Character>(luaVM, "Character", nullptr, characterMetatable, nullptr, nullptr);
}
