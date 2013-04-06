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
   if(character == NULL)
   {
      return lua_error(luaVM);
   }

   std::string name = character->getName();
   lua_pushstring(luaVM, name.c_str());
   return 1;
}

static int CharacterL_AddSkill(lua_State* luaVM)
{
   Character* character = luaW_check<Character>(luaVM, 1);
   if(character == NULL)
   {
      return lua_error(luaVM);
   }

   int skillId;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "id", skillId))
   {
      return lua_error(luaVM);
   }

   bool success = character->addSkill(lua_tonumber(luaVM, 2));
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
