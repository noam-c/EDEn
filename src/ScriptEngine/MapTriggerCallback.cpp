/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
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
   m_registryIndex(luaL_ref(luaVM, LUA_REGISTRYINDEX))
{
}

MapTriggerCallback::~MapTriggerCallback()
{
   luaL_unref(m_luaVM, LUA_REGISTRYINDEX, m_registryIndex);
}

int MapTriggerCallback::operator()(Actor* actor)
{
   if(m_registryIndex != LUA_NOREF)
   {
      lua_rawgeti(m_luaVM, LUA_REGISTRYINDEX, m_registryIndex);
      luaW_push<Actor>(m_luaVM, actor);
      return lua_pcall(m_luaVM, 1, 0, 0);
   }

   return lua_error(m_luaVM);
}
