/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ScriptEngine.h"
#include "NPC.h"

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

static int luaNarrate(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->narrate(luaVM);
}

static int luaSay(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->say(luaVM);
}

static int luaSetRegion(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->setRegion(luaVM);
}

static int luaPlaySound(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->playSound(luaVM);
}

static int luaPlayMusic(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->playMusic(luaVM);
}

static int luaStopMusic(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->stopMusic(luaVM);
}

static int luaIsMusicPlaying(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->isMusicPlaying(luaVM);
}

static int luaDelay(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->delay(luaVM);
}

static int luaRandom(lua_State* luaVM)
{
   return ScriptEngine::getScriptEngineForVM(luaVM)->generateRandom(luaVM);
}

static inline void registerFunction(lua_State* luaVM, const char* luaName, lua_CFunction function) {
   DEBUG("Registering function: %s", luaName);
   lua_register(luaVM, luaName, function);
}

void ScriptEngine::registerFunctions()
{
   registerFunction(m_luaVM, "narrate", luaNarrate);
   registerFunction(m_luaVM, "say", luaSay);
   registerFunction(m_luaVM, "playSound", luaPlaySound);
   registerFunction(m_luaVM, "playMusic", luaPlayMusic);
   registerFunction(m_luaVM, "stopMusic", luaStopMusic);
   registerFunction(m_luaVM, "isMusicPlaying", luaIsMusicPlaying);
   registerFunction(m_luaVM, "delay", luaDelay);
   registerFunction(m_luaVM, "random", luaRandom);

   // Tile Engine functions
   registerFunction(m_luaVM, "setRegion", luaSetRegion);
}
