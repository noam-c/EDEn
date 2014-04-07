/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "LuaWrapper.hpp"
#include "MapTriggerCallback.h"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

MapTriggerCallback::MapTriggerCallback(lua_State* luaVM) :
   m_luaVM(luaVM),
   m_registryIndex(lua_ref(luaVM, true))
{
}

MapTriggerCallback::~MapTriggerCallback()
{
   lua_unref(m_luaVM, m_registryIndex);
}

int MapTriggerCallback::operator()(Actor* actor)
{
   if(m_registryIndex != LUA_NOREF)
   {
      lua_getref(m_luaVM, m_registryIndex);
      luaW_push<Actor>(m_luaVM, actor);
      return lua_pcall(m_luaVM, 1, 0, 0);
   }
   
   return lua_error(m_luaVM);
}
