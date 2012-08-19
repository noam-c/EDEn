/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "NPCScript.h"
#include "NPC.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

const char* NPCScript::FUNCTION_NAMES[] = { "idle", "activate" };

NPCScript::NPCScript(lua_State* luaVM, const std::string& scriptPath, NPC* npc) : Script(scriptPath), npc(npc), activated(false), finished(false)
{
   luaStack = lua_newthread(luaVM);

   // Run through the script to gather all the NPC functions
   DEBUG("Script ID %d loading functions from %s", getId(), scriptPath.c_str());

   int result = luaL_dofile(luaStack, scriptPath.c_str());

   if(result != 0)
   {
      DEBUG("Error loading NPC functions for %s: %s", npc->getName().c_str(), lua_tostring(luaStack, -1));
   }

   // All the below code simply takes all the global functions that the script
   // created, and pushes them into a table that uses this NPC's unique
   // identifier. Then it removes those global functions for safety.

   // Sure, this would be much easier to do in the Lua script itself, but
   // I want to have absolutely no boilerplate code or metatable BS inside the
   // Lua scripts themselves; I want the .lua files to all be approachable by
   // novice programmers and even non-programmers, and I don't want them to see
   // "black box" code in each file. This should never cause a problem for the
   // flexibility of the scripts, because anything you can do in Lua, you can
   // do using the Lua/C++ API.

   functionExists = new bool[NUM_FUNCTIONS];

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

         // The function is valid for the NPC
         functionExists[i] = true;

         DEBUG("Function %s was found and loaded", FUNCTION_NAMES[i]);
      }
   }

   // Push the table into the global space with the file path as the name
   lua_setglobal(luaStack, scriptName.c_str());

}

bool NPCScript::callFunction(NPCFunction function)
{
   if(functionExists[function])
   {
      // Load the table of Lua functions for this NPC
      lua_getglobal(luaStack, scriptName.c_str());

      // Grab the function name
      const char* functionName = FUNCTION_NAMES[function];

      DEBUG("NPC %s running function %s", npc->getName().c_str(), functionName);

      // Get the function from the NPC function table and push it on the stack
      lua_pushstring(luaStack, functionName);
      lua_gettable(luaStack, -2);

      // Push NPC as argument
      luaW_push<Actor>(luaStack, npc);

      // Run the script
      return runScript(1);
   }

   return true;
}

bool NPCScript::resume(long timePassed)
{
   if(finished) return true;
   
   if(activated)
   {
      activated = false;
      callFunction(ACTIVATE);
   }

   if(running)
   {
      DEBUG("NPC Coroutine %d resuming running script.", getId());
      runScript();
   }
   else
   {
      if(npc->isIdle())
      {
         callFunction(IDLE);
      }
   }

   return false;
}

void NPCScript::activate()
{
   activated = true;
}

void NPCScript::finish()
{
   finished = true;
}

NPCScript::~NPCScript()
{
   /** \todo Check if this cleanup is appropriate. */
   // Set the function table to nil so that it gets garbage collected
   // lua_pushnil(luaStack);
   // lua_setglobal(luaStack, scriptPath.c_str());

   delete [] functionExists;
}
