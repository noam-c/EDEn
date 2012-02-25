/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ScriptEngine.h"
#include "TileEngine.h"
#include "Scheduler.h"
#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"
#include "Timer.h"
#include "NPC.h"
#include "FileScript.h"
#include "StringScript.h"
#include "ScriptFactory.h"

#include "LuaPlayerCharacter.h"
#include "LuaActor.h"
#include "LuaTileEngine.h"

#include "LuaWrapper.hpp"

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

ScriptEngine::ScriptEngine(TileEngine& tileEngine, PlayerData& playerData, Scheduler& scheduler)
                                  : tileEngine(tileEngine), playerData(playerData), scheduler(scheduler)
{
   luaVM = luaL_newstate();

   if(luaVM == NULL)
   {
      // An error occurred
      T_T("Unable to initialize Lua state machine");
   }

   //initialize standard Lua libraries
   luaL_openlibs(luaVM);

   // register game functions with Lua
   registerFunctions();

   // Push this engine instance as a global pointer for the Lua VM
   // This allows global C functions in our API to reference this instance
   // (thus eliminating the need for a global ScriptEngine instance or singleton)
   lua_pushlightuserdata(luaVM, this);
   lua_setglobal(luaVM, SCRIPT_ENG_LUA_NAME);
   
   luaopen_TileEngine(luaVM);
   luaW_push<TileEngine>(luaVM, &tileEngine);
   lua_setglobal(luaVM, "map");
   
   luaopen_Actor(luaVM);

   luaopen_PlayerCharacter(luaVM);
   luaW_push<PlayerCharacter>(luaVM, tileEngine.getPlayerCharacter());
   lua_setglobal(luaVM, "player");
}

int ScriptEngine::narrate(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);
   bool waitForFinish = false;

   int callResult = 0;

   if(nargs > 0)
   {
      const char* speech = lua_tostring(luaStack, 1);
      if(nargs == 2)
      {
         waitForFinish = (lua_toboolean(luaStack, 2) == 1);
      }

      Task* task = Task::getNextTask(scheduler);
      if(waitForFinish)
      {
         callResult = scheduler.block(task);
      }

      DEBUG("Narrating text: %s", speech);
      tileEngine.dialogueNarrate(speech, task);
   }

   return callResult;
}

int ScriptEngine::say(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);
   bool waitForFinish = false;

   int callResult = 0;

   if(nargs > 0)
   {
      const char* speech = lua_tostring(luaStack, 1);
      if(nargs == 2)
      {
         waitForFinish = (lua_toboolean(luaStack, 2) == 1);
      }

      Task* task = Task::getNextTask(scheduler);
      if(waitForFinish)
      {
         callResult = scheduler.block(task);
      }

      DEBUG("Saying text: %s", speech);
      tileEngine.dialogueSay(speech, task);
   }

   return callResult;
}

int ScriptEngine::playSound(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);
   bool waitForFinish = false;

   if(nargs > 0)
   {
      std::string soundName(lua_tostring(luaStack, 1));
      DEBUG("Playing sound: %s", soundName.c_str());

      if(nargs == 2)
      {
         waitForFinish = (lua_toboolean(luaStack, 2) == 1);
      }

      Task* task = Task::getNextTask(scheduler);

      DEBUG("Playing sound: %s", soundName.c_str());
      Sound* sound = ResourceLoader::getSound(soundName);
      sound->play(task);

      if(waitForFinish)
      {
         return scheduler.block(task);
      }
   }

   return 0;
}

int ScriptEngine::playMusic(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);

   if(nargs > 0)
   {
      std::string musicName(lua_tostring(luaStack, 1));
      DEBUG("Playing music: %s", musicName.c_str());

      Music* song = ResourceLoader::getMusic(musicName);
      song->play();
   }

   return 0;
}

int ScriptEngine::stopMusic(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);

   if(nargs == 0)
   {
      DEBUG("Stopping music.");
      Music::stopMusic();
   }

   return 0;
}

int ScriptEngine::delay(lua_State* luaStack)
{
   long timeToWait = (long)lua_tonumber(luaStack, 1);
   DEBUG("Waiting %d milliseconds", timeToWait);
   
   Timer* waitTimer = new Timer(timeToWait);
   scheduler.start(waitTimer);
   
   return scheduler.join(waitTimer);
}

int ScriptEngine::generateRandom(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);
   int min, max;
   
   switch(nargs)
   {
      case 1:
      {
         min = 0;
         max = (int)luaL_checknumber(luaStack, 1);
      }
      case 2:
      {
         min = (int)luaL_checknumber(luaStack, 1);
         max = (int)luaL_checknumber(luaStack, 2);
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

int ScriptEngine::setRegion(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);
   if(nargs > 0)
   {
      std::string regionName(lua_tostring(luaStack, 1));
      DEBUG("Setting region: %s", regionName.c_str());

      if(!tileEngine.setRegion(regionName))
      {
         /** \todo Report an error to Lua, perhaps throw a ScriptException? */
      }

      std::string mapName = tileEngine.getMapName();
      return runMapScript(regionName, mapName);
   }

   return 0;
}

NPCScript* ScriptEngine::getNPCScript(NPC* npc, const std::string& regionName, const std::string& mapName, const std::string& npcName)
{
   return ScriptFactory::getNPCScript(luaVM, npc, regionName, mapName, npcName);
}

int ScriptEngine::runMapScript(const std::string& regionName, const std::string& mapName)
{
   Script* mapScript = ScriptFactory::getMapScript(luaVM, regionName, mapName);
   return runScript(mapScript);
}

int ScriptEngine::runChapterScript(const std::string& chapterName)
{
   Script* chapterScript = ScriptFactory::getChapterScript(luaVM, chapterName);
   return runScript(chapterScript);
}

int ScriptEngine::runScript(Script* script)
{
   scheduler.start(script);

   if(scheduler.hasRunningThread())
   {
      return scheduler.join(script);
   }

   return 0;
}

int ScriptEngine::runScriptString(const std::string& scriptString)
{
   DEBUG("Running script string: %s", scriptString.c_str());
   StringScript* newScript = new StringScript(luaVM, scriptString);
   scheduler.start(newScript);

   if(scheduler.hasRunningThread())
   {
      return scheduler.join(newScript);
   }

   return 0;
}

void ScriptEngine::callFunction(lua_State* thread, const char* funcName)
{
   // push the function onto the stack and then resume the thread from the
   // start of the function 
   lua_getfield(thread, LUA_GLOBALSINDEX, funcName);
   lua_resume(thread, 0);
}

std::string ScriptEngine::getScriptPath(const std::string& scriptName)
{
   return scriptName + ".lua";
}

ScriptEngine::~ScriptEngine()
{
   if(luaVM)
   {
      DEBUG("Destroying Lua state machine...");
      lua_close(luaVM);
   }
}
