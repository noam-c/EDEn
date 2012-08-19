/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
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

#define REGISTER(luaName, function) DEBUG("Registering function: %s", luaName); \
                                    lua_register(luaVM, luaName, function)

static ScriptEngine* getEngine(lua_State* luaVM)
{
   lua_getglobal(luaVM, SCRIPT_ENG_LUA_NAME);
   ScriptEngine* engine = static_cast<ScriptEngine*>(lua_touserdata(luaVM, lua_gettop(luaVM)));
   lua_pop(luaVM, 1);
   return engine;
}

static int luaNarrate(lua_State* luaVM)
{
   return getEngine(luaVM)->narrate(luaVM);
}

static int luaSay(lua_State* luaVM)
{
   return getEngine(luaVM)->say(luaVM);
}

static int luaSetRegion(lua_State* luaVM)
{
   return getEngine(luaVM)->setRegion(luaVM);
}

static int luaPlaySound(lua_State* luaVM)
{
   return getEngine(luaVM)->playSound(luaVM);
}

static int luaPlayMusic(lua_State* luaVM)
{
   return getEngine(luaVM)->playMusic(luaVM);
}

static int luaStopMusic(lua_State* luaVM)
{
   return getEngine(luaVM)->stopMusic(luaVM);
}

static int luaDelay(lua_State* luaVM)
{
   return getEngine(luaVM)->delay(luaVM);
}

static int luaRandom(lua_State* luaVM)
{
   return getEngine(luaVM)->generateRandom(luaVM);
}

void ScriptEngine::registerFunctions()
{
   REGISTER("narrate", luaNarrate);
   REGISTER("say", luaSay);
   REGISTER("playSound", luaPlaySound);
   REGISTER("playMusic", luaPlayMusic);
   REGISTER("stopMusic", luaStopMusic);
   REGISTER("delay", luaDelay);
   REGISTER("random", luaRandom);

   // Tile Engine functions
   REGISTER("setRegion", luaSetRegion);
}
