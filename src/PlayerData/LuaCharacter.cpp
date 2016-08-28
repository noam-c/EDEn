/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
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

static int CharacterL_Stat(lua_State* luaVM)
{
   auto character = luaW_check<Character>(luaVM, 1);
   if(character == nullptr)
   {
      return lua_error(luaVM);
   }
   
   std::string name;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "name", name))
   {
      return lua_error(luaVM);
   }

   int value;
   if(ScriptUtilities::getParameter(luaVM, 2, 2, "value", value))
   {
      character->setStatAttribute(name, value);
      return 0;
   }

   if(character->hasStatAttribute(name))
   {
      lua_pushnumber(luaVM, character->getStatAttribute(name));
   }
   else
   {
      lua_pushnil(luaVM);
   }

   return 1;
}

static luaL_Reg characterMetatable[] =
{
   { "getName", CharacterL_GetName },
   { "stat", CharacterL_Stat },
   { nullptr, nullptr }
};

void luaopen_Character(lua_State* luaVM)
{
   luaW_register<Character>(luaVM, "Character", nullptr, characterMetatable, nullptr, nullptr);
}
