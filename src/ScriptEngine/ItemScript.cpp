/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ItemScript.h"
#include "Item.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_METADATA;

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

const char* ItemScript::FUNCTION_NAMES[] = { "onMenuUse", "onFieldUse", "onBattleUse" };

ItemScript::ItemScript(lua_State* luaVM, const std::string& scriptPath, const Item& item) :
   Script(scriptPath),
   functionExists(NUM_FUNCTIONS),
   item(item)
{
   luaStack = lua_newthread(luaVM);

   // Run through the script to gather all the item's functions
   DEBUG("Script ID %d loading functions from %s", getId(), scriptPath.c_str());

   int result = luaL_dofile(luaVM, scriptPath.c_str());

   if(result != 0)
   {
      DEBUG("Error loading item functions for item ID %d: %s", item.getId(), lua_tostring(luaStack, -1));
   }

   // All the below code simply takes all the global functions that the script
   // created, and pushes them into a table that uses the item's unique
   // identifier. Then it removes those global functions for safety.

   // Sure, this would be much easier to do in the Lua script itself, but
   // I want to have absolutely no boilerplate code or metatable BS inside the
   // Lua scripts themselves; I want the .lua files to all be approachable by
   // novice programmers and even non-programmers, and I don't want them to see
   // "black box" code in each file. This should never cause a problem for the
   // flexibility of the scripts, because anything you can do in Lua, you can
   // do using the Lua/C++ API.

   // Create a table for this NPC's functions
   lua_createtable(luaStack, 0, NUM_FUNCTIONS);

   for(int i = 0; i < NUM_FUNCTIONS; ++i)
   {
      DEBUG("Checking for function %s.", FUNCTION_NAMES[i]);

      // Push the function name into the table
      lua_pushstring(luaStack, FUNCTION_NAMES[i]);

      // Push the function pointer onto the stack
      lua_getglobal(luaStack, FUNCTION_NAMES[i]);

      // Do a type-check here and push an empty function if needed
      if(!lua_isfunction(luaStack, -1))
      {
         // Pop off the bad value and the function name
         lua_pop(luaStack, 2);

         // This function is not available for the NPC
         functionExists[i] = false;

         DEBUG("%s was not found to be a function!", FUNCTION_NAMES[i]);
      }
      else
      {
         // Grabs the table explicitly, and uses the string function name as a key
         // to push in the global function we found (pops string and function off the stack)
         lua_settable(luaStack, -3);

         // Remove the function from the global table so nobody else accidentally runs into it
         lua_pushnil(luaStack);
         lua_setglobal(luaStack, FUNCTION_NAMES[i]);

         // The function is valid for the item
         functionExists[i] = true;

         DEBUG("Function %s was found and loaded", FUNCTION_NAMES[i]);
      }
   }

   // Push the table into the global space with the file path as the name
   lua_setglobal(luaStack, scriptName.c_str());
}

ItemScript::~ItemScript()
{
   /** \todo Check if this cleanup is appropriate. */
   // Set the function table to nil so that it gets garbage collected
   // lua_pushnil(luaStack);
   // lua_setglobal(luaStack, scriptPath.c_str());
}

bool ItemScript::callFunction(ItemFunction function)
{
   if(functionExists[function])
   {
      // Load the table of Lua functions for this NPC
      lua_getglobal(luaStack, scriptName.c_str());

      // Grab the function name
      const char* functionName = FUNCTION_NAMES[function];

      DEBUG("Item ID %d running function %s", item.getId(), functionName);

      // Get the function from the NPC function table and push it on the stack
      lua_pushstring(luaStack, functionName);
      lua_gettable(luaStack, -2);

      // Run the script
      return runScript();
   }

   return true;
}

bool ItemScript::onMenuUse()
{
   return callFunction(MENU_USE);
}

bool ItemScript::onFieldUse()
{
   return callFunction(FIELD_USE);
}

bool ItemScript::onBattleUse()
{
   return callFunction(BATTLE_USE);
}
