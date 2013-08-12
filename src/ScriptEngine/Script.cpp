/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
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

Script::Script(const std::string& name) :
   m_scriptName(name),
   m_running(false)
{
}

Script::~Script()
{
}

bool Script::runScript(int numArgs)
{
   if(!m_luaStack)
   {
      T_T("Attempting to run an uninitialized script!");
   }

   m_running = true;

   DEBUG("Resuming script with name %s, coroutine ID %d...", m_scriptName.c_str(), m_coroutineId);
   DEBUG("Lua Coroutine Address: 0x%x", m_luaStack);

   int returnCode = lua_resume(m_luaStack, numArgs);
   switch(returnCode)
   {
      case 0:
      {
         DEBUG("Script %d finished.", m_coroutineId);
         m_running = false;
         return true;
      }
      case LUA_YIELD:
      {
         DEBUG("Script %d yielded.", m_coroutineId);
         return false;
      }
      default:
      {
         // An error occurred: Print out the error message
         DEBUG("Error running script: %s", lua_tostring(m_luaStack, -1));
         m_running = false;
         T_T("An error occured running this script.");
         return true;
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
   return lua_yield(m_luaStack, 0);
}
