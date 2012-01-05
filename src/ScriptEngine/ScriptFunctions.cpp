/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ScriptEngine.h"
#include "TileEngine.h"
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

static int luaAddNPC(lua_State* luaVM)
{
   return getEngine(luaVM)->addNPC(luaVM);
}

static int luaDelay(lua_State* luaVM)
{
   return getEngine(luaVM)->delay(luaVM);
}

static int luaNPCMove(lua_State* luaVM)
{
   return getEngine(luaVM)->moveNPC(luaVM);
}

static int luaNPCSetSprite(lua_State* luaVM)
{
   return getEngine(luaVM)->setNPCSprite(luaVM);
}

static int luaNPCSetAnimation(lua_State* luaVM)
{
   return getEngine(luaVM)->setNPCAnimation(luaVM);
}

static int luaNPCChangeSpritesheet(lua_State* luaVM)
{
   return getEngine(luaVM)->changeNPCSpritesheet(luaVM);
}

static int luaNPCLookAtPlayer(lua_State* luaVM)
{
   return getEngine(luaVM)->turnNPCTowardsPlayer(luaVM);
}

static int luaTilesToPixels(lua_State* luaVM)
{ 
   int numInTiles = (int)luaL_checknumber(luaVM, 1);
   lua_pushnumber(luaVM, numInTiles * TileEngine::TILE_SIZE);
   return 1;
}

static int luaRandom(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   int min, max;

   switch(nargs)
   {
      case 1:
      {
         min = 0;
         max = (int)luaL_checknumber(luaVM, 1);
      }
      case 2:
      {
         min = (int)luaL_checknumber(luaVM, 1);
         max = (int)luaL_checknumber(luaVM, 2);
         break;
      }
      default:
      {
         /** \todo Error case. */
         min = 0;
         max = 0;
         break;
      }
   }

   /** \todo Random number generation is currently unseeded. Use a good seed. */
   lua_pushnumber(luaVM, (rand() % (max - min)) + min);
   return 1;
}

void ScriptEngine::registerFunctions()
{
   // Tile Engine functions
   REGISTER("narrate", luaNarrate);
   REGISTER("say", luaSay);
   REGISTER("setRegion", luaSetRegion);
   REGISTER("playSound", luaPlaySound);
   REGISTER("playMusic", luaPlayMusic);
   REGISTER("stopMusic", luaStopMusic);
   REGISTER("addNPC", luaAddNPC);
   REGISTER("delay", luaDelay);
   REGISTER("move", luaNPCMove);
   REGISTER("changeSpritesheet", luaNPCChangeSpritesheet);
   REGISTER("setSprite", luaNPCSetSprite);
   REGISTER("setAnimation", luaNPCSetAnimation);
   REGISTER("lookAtPlayer", luaNPCLookAtPlayer);

   // Utility functions (calculations)
   REGISTER("tilesToPixels", luaTilesToPixels);
   REGISTER("random", luaRandom);

//   REGISTER("setMap", luaSetMap);
}
