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

#include "LuaWrapper.hpp"

#include "DebugUtils.h"
const int debugFlag = DEBUG_SCRIPT_ENG;

Script::Script(const std::string& name) :
   m_scriptName(name),
   m_running(false)
{
}

Script::~Script() = default;

/**
 * Operation to push all result parameters onto a given Lua coroutine's stack.
 * Used to push an arbitrary bag of return values or function parameters
 * onto a stack prior to resumption of the Lua coroutine.
 *
 * @author Noam Chitayat
 */
class PushOnLuaStackOp : public ICoroutineResultOp
{
   lua_State* m_luaVM;

   public:
      PushOnLuaStackOp(lua_State* luaVM) :
         m_luaVM(luaVM)
      {
      }

      void operator()(bool arg)
      {
         DEBUG("Pushing boolean: %d", arg);
         lua_pushboolean(m_luaVM, arg);
      }

      void operator()(int arg)
      {
         DEBUG("Pushing number: %d", arg);
         lua_pushnumber(m_luaVM, arg);
      }
};

bool Script::runScript(int numArgs)
{
   if(!m_luaStack)
   {
      T_T("Attempting to run an uninitialized script!");
   }

   if (m_running)
   {
      std::unique_ptr<ICoroutineResults> args = retrieveResults();

      if (args)
      {
         PushOnLuaStackOp op(m_luaStack);
         args->forEach(op);
         numArgs = args->getSize();
      }
   }

   m_running = true;

   DEBUG("Resuming script with name %s, coroutine ID %d...", m_scriptName.c_str(), m_coroutineId);
   DEBUG("Lua Coroutine Address: 0x%x", m_luaStack);

   int returnCode = lua_resume(m_luaStack, nullptr, numArgs);
   switch(returnCode)
   {
      case LUA_OK:
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
   return runScript();
}

int Script::yield(int numResults)
{
   return lua_yield(m_luaStack, numResults);
}
