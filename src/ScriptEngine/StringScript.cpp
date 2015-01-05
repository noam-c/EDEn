/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
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
   m_luaStack = lua_newthread(luaVM);
   luaL_loadstring(m_luaStack, scriptString.c_str());
}
