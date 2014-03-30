/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "LuaCharacterRoster.h"
#include "CharacterRoster.h"
#include "ScriptUtilities.h"

#include <string>

static int CharacterListL_CreateCharacter(lua_State* luaVM)
{
   CharacterRoster* characterRoster = luaW_check<CharacterRoster>(luaVM, 1);
   if(characterRoster == nullptr)
   {
      return lua_error(luaVM);
   }

   std::string characterId;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "id", characterId))
   {
      return lua_error(luaVM);
   }

   Character* createdCharacter = characterRoster->loadNewCharacter(characterId);
   luaW_push<Character>(luaVM, createdCharacter);
   return 1;
}

static int CharacterListL_AddToParty(lua_State* luaVM)
{
   CharacterRoster* characterRoster = luaW_check<CharacterRoster>(luaVM, 1);
   if(characterRoster == nullptr)
   {
      return lua_error(luaVM);
   }

   Character* characterToAdd;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "character", characterToAdd))
   {
      return lua_error(luaVM);
   }

   characterRoster->addToParty(characterToAdd);
   return 0;
}

static luaL_reg characterRosterMetatable[] =
{
   { "createCharacter", CharacterListL_CreateCharacter },
   { "addToParty", CharacterListL_AddToParty },
   { nullptr, nullptr }
};

void luaopen_CharacterRoster(lua_State* luaVM)
{
   luaW_register<CharacterRoster>(luaVM, "CharacterRoster", nullptr, characterRosterMetatable, nullptr, nullptr);
}
