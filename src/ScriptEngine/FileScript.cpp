/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "FileScript.h"
#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_SCRIPT_ENG

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

FileScript::FileScript(lua_State* luaVM, const std::string& scriptPath) :
   Script(scriptPath)
{
   m_luaStack = lua_newthread(luaVM);
}

bool FileScript::initialize()
{
   DEBUG("Script ID %d loading file %s", getId(), m_scriptName.c_str());

   int result = luaL_loadfile(m_luaStack, m_scriptName.c_str());
   if(result != 0)
   {
      const char* errorString = luaL_checkstring(m_luaStack, 1);
      DEBUG("%s", errorString);

      return false;
   }

   return true;
}
