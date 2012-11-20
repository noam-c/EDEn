/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "StringScript.h"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

StringScript::StringScript(lua_State* luaVM, const std::string& scriptString) :
   Script(scriptString)
{
   luaStack = lua_newthread(luaVM);
   luaL_loadstring(luaStack, scriptString.c_str());
}

StringScript::~StringScript()
{
}
