/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Script.h"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

#include "DebugUtils.h"
const int debugFlag = DEBUG_SCRIPT_ENG;

Script::Script(const std::string& name) : scriptName(name), running(false)
{
}

bool Script::runScript()
{
   if(!luaStack)
   {
      T_T("Attempting to run an uninitialized script!");
   }

   running = true;

   DEBUG("Resuming script with name %s, thread ID %d...", scriptName.c_str(), threadId);
   DEBUG("Lua Thread Address: 0x%x", luaStack);

   int returnCode = lua_resume(luaStack, 0);
   switch(returnCode)
   {
      case 0:
      {
         DEBUG("Script %d finished.", threadId);
         running = false;
         return true;
      }
      case LUA_YIELD:
      {
         DEBUG("Script %d yielded.", threadId);
         return false;
      }
      default:
      {
         // An error occurred: Print out the error message
         DEBUG("Error running script: %s", lua_tostring(luaStack, -1));
         running = false;
         T_T("An error occured running this script.");
      }
   }

}

bool Script::resume(long /*timePassed*/)
{
   bool result = runScript();
   return result;
}

int Script::yield()
{
   return lua_yield(luaStack, 0);
}

Script::~Script()
{
}
