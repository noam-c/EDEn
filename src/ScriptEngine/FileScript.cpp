/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "FileScript.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SCRIPT_ENG;

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
   DEBUG("Script ID %d loading file %s", getId(), scriptPath.c_str());
   luaL_loadfile(m_luaStack, scriptPath.c_str());
}

FileScript::~FileScript()
{
}
