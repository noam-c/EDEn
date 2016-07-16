/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "CharacterDataScript.h"

#include "Character.h"
#include "EnumUtils.h"
#include "LuaWrapper.hpp"
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_METADATA

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

const std::string characterDataScriptPath = "data/scripts/characters/";
const char* CharacterDataScript::FUNCTION_NAMES[] = { "initialize" };

CharacterDataScript::CharacterDataScript(lua_State* luaVM, const std::string& scriptName) :
   Script(scriptName),
   m_functionExists(EnumUtils::toNumber(CharacterDataFunction::NUM_FUNCTIONS))
{
   m_luaStack = lua_newthread(luaVM);

   auto scriptPath = characterDataScriptPath + scriptName + ".lua";

   // Run through the script to gather all the usable's functions
   DEBUG("Script ID %d loading functions from %s", getId(), scriptPath.c_str());

   int result = luaL_dofile(luaVM, scriptPath.c_str());

   if(result != 0)
   {
      DEBUG("Error loading character data script ID '%s': %s", scriptPath.c_str(), lua_tostring(m_luaStack, -1));
   }

   // All the below code simply takes all the global functions that the script
   // created, and pushes them into a table that uses the usable's unique
   // identifier. Then it removes those global functions for safety.

   // Sure, this would be much easier to do in the Lua script itself, but
   // I want to have absolutely no boilerplate code or metatable BS inside the
   // Lua scripts themselves; I want the .lua files to all be approachable by
   // novice programmers and even non-programmers, and I don't want them to see
   // "black box" code in each file. This should never cause a problem for the
   // flexibility of the scripts, because anything you can do in Lua, you can
   // do using the Lua/C++ API.

   // Create a table for this NPC's functions
   lua_createtable(m_luaStack, 0, m_functionExists.size());

   for(size_t i = 0; i < m_functionExists.size(); ++i)
   {
      DEBUG("Checking for function %s.", FUNCTION_NAMES[i]);

      // Push the function name into the table
      lua_pushstring(m_luaStack, FUNCTION_NAMES[i]);

      // Push the function pointer onto the stack
      lua_getglobal(m_luaStack, FUNCTION_NAMES[i]);

      // Do a type-check here and push an empty function if needed
      if(!lua_isfunction(m_luaStack, -1))
      {
         // Pop off the bad value and the function name
         lua_pop(m_luaStack, 2);

         // This function is not available for the NPC
         m_functionExists[i] = false;

         DEBUG("%s was not found to be a function!", FUNCTION_NAMES[i]);
      }
      else
      {
         // Grabs the table explicitly, and uses the string function name as a key
         // to push in the global function we found (pops string and function off the stack)
         lua_settable(m_luaStack, -3);

         // Remove the function from the global table so nobody else accidentally runs into it
         lua_pushnil(m_luaStack);
         lua_setglobal(m_luaStack, FUNCTION_NAMES[i]);

         // The function is valid for the usable
         m_functionExists[i] = true;

         DEBUG("Function %s was found and loaded", FUNCTION_NAMES[i]);
      }
   }

   // Push the table into the global space with the file path as the name
   lua_setglobal(m_luaStack, m_scriptName.c_str());
}

CharacterDataScript::~CharacterDataScript()
{
   /** \todo Check if this cleanup is appropriate. */
   // Set the function table to nil so that it gets garbage collected
   // lua_pushnil(luaStack);
   // lua_setglobal(luaStack, scriptPath.c_str());
}

bool CharacterDataScript::callFunction(CharacterDataFunction function, Character* usingCharacter)
{
   auto funcIndex = EnumUtils::toNumber(function);
   if(m_functionExists[funcIndex])
   {
      // Load the table of Lua functions
      lua_getglobal(m_luaStack, m_scriptName.c_str());

      // Grab the function name
      const auto functionName = FUNCTION_NAMES[funcIndex];

      DEBUG("Running CharacterData function %s", functionName);

      // Get the function from the NPC function table and push it on the stack
      lua_pushstring(m_luaStack, functionName);
      lua_gettable(m_luaStack, -2);

      int numArgs = 0;
      if(usingCharacter != nullptr)
      {
         luaW_push(m_luaStack, usingCharacter);
         numArgs = 1;
      }

      // Run the script
      return runScript(numArgs);
   }

   return true;
}

bool CharacterDataScript::initialize(Character* usingCharacter)
{
   return callFunction(CharacterDataFunction::INITIALIZE, usingCharacter);
}
