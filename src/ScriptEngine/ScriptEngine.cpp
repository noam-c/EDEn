/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ScriptEngine.h"
#include "TileEngine.h"
#include "PlayerData.h"
#include "GameContext.h"
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

#include "LuaCharacter.h"
#include "LuaCharacterRoster.h"
#include "LuaQuest.h"
#include "LuaInventory.h"

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

ScriptEngine::ScriptEngine(GameContext& gameContext) :
   gameContext(gameContext)
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
   luaopen_Actor(luaVM);
   luaopen_PlayerCharacter(luaVM);
   luaopen_Quest(luaVM);
   luaopen_Inventory(luaVM);
   luaopen_Character(luaVM);
   luaopen_CharacterRoster(luaVM);
}

ScriptEngine::~ScriptEngine()
{
   if(luaVM)
   {
      DEBUG("Destroying Lua state machine...");
      lua_close(luaVM);
      DEBUG("Lua state machine destroyed.");
   }
}

void ScriptEngine::setTileEngine(TileEngine* engine)
{
   tileEngine = engine;
   if(tileEngine != NULL)
   {
      luaW_push(luaVM, tileEngine);
      lua_setglobal(luaVM, "map");

      luaW_push(luaVM, tileEngine->getPlayerCharacter());
      lua_setglobal(luaVM, "playerSprite");
   }
   else
   {
      lua_pushnil(luaVM);
      lua_setglobal(luaVM, "map");

      lua_pushnil(luaVM);
      lua_setglobal(luaVM, "playerSprite");
   }
}

void ScriptEngine::setPlayerData(PlayerData* playerData)
{
   this->playerData = playerData;
   if(this->playerData != NULL)
   {
      luaW_push(luaVM, this->playerData->getRootQuest());
      lua_setglobal(luaVM, "quests");

      luaW_push(luaVM, this->playerData->getInventory());
      lua_setglobal(luaVM, "inventory");

      luaW_push(luaVM, this->playerData->getRoster());
      lua_setglobal(luaVM, "roster");
   }
   else
   {
      lua_pushnil(luaVM);
      lua_setglobal(luaVM, "quests");

      lua_pushnil(luaVM);
      lua_setglobal(luaVM, "inventory");

      lua_pushnil(luaVM);
      lua_setglobal(luaVM, "roster");
   }
}

int ScriptEngine::narrate(lua_State* luaStack)
{
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   if (scheduler == NULL)
   {
      return 0;
   }

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

      Task* task = Task::getNextTask(*scheduler);
      if(waitForFinish)
      {
         callResult = scheduler->block(task);
      }

      DEBUG("Narrating text: %s", speech);
      tileEngine->dialogueNarrate(speech, task);
   }

   return callResult;
}

int ScriptEngine::say(lua_State* luaStack)
{
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   if (scheduler == NULL)
   {
      return 0;
   }

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

      Task* task = Task::getNextTask(*scheduler);
      if(waitForFinish)
      {
         callResult = scheduler->block(task);
      }

      DEBUG("Saying text: %s", speech);
      tileEngine->dialogueSay(speech, task);
   }

   return callResult;
}

int ScriptEngine::playSound(lua_State* luaStack)
{
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   if (scheduler == NULL)
   {
      return 0;
   }

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

      Task* task = Task::getNextTask(*scheduler);

      DEBUG("Playing sound: %s", soundName.c_str());
      Sound* sound = ResourceLoader::getSound(soundName);
      sound->play(task);

      if(waitForFinish)
      {
         return scheduler->block(task);
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
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   if (scheduler == NULL)
   {
      return 0;
   }

   long timeToWait = (long)lua_tonumber(luaStack, 1);
   DEBUG("Waiting %d milliseconds", timeToWait);
   
   Timer* waitTimer = new Timer(timeToWait);
   scheduler->start(waitTimer);
   
   return scheduler->join(waitTimer);
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
         break;
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
   lua_pushnumber(luaStack, (rand() % (max - min)) + min);

   return 1;
}

int ScriptEngine::setRegion(lua_State* luaStack)
{
   int nargs = lua_gettop(luaStack);
   if(nargs > 0)
   {
      std::string regionName(lua_tostring(luaStack, 1));
      DEBUG("Setting region: %s", regionName.c_str());

      return tileEngine->setRegion(regionName);
   }

   return 0;
}

NPCScript* ScriptEngine::createNPCCoroutine(NPC* npc, const std::string& regionName, const std::string& mapName)
{
   return ScriptFactory::createNPCCoroutine(luaVM, npc, regionName, mapName);
}

ItemScript* ScriptEngine::createItemScript(const Item& item) const
{
   return ScriptFactory::getItemScript(luaVM, item);
}

int ScriptEngine::runMapScript(const std::string& regionName, const std::string& mapName, Scheduler& scheduler)
{
   Script* mapScript = ScriptFactory::getMapScript(luaVM, regionName, mapName);
   return runScript(mapScript, scheduler);
}

int ScriptEngine::runChapterScript(const std::string& chapterName, Scheduler& scheduler)
{
   Script* chapterScript = ScriptFactory::getChapterScript(luaVM, chapterName);
   return runScript(chapterScript, scheduler);
}

int ScriptEngine::runScript(Script* script, Scheduler& scheduler)
{
   scheduler.start(script);

   if(scheduler.hasRunningCoroutine())
   {
      return scheduler.join(script);
   }

   return 0;
}

int ScriptEngine::runScriptString(const std::string& scriptString, Scheduler& scheduler)
{
   DEBUG("Running script string: %s", scriptString.c_str());
   StringScript* newScript = new StringScript(luaVM, scriptString);
   scheduler.start(newScript);

   if(scheduler.hasRunningCoroutine())
   {
      return scheduler.join(newScript);
   }

   return 0;
}

void ScriptEngine::callFunction(lua_State* coroutine, const char* funcName)
{
   // push the function onto the stack and then resume the coroutine from the
   // start of the function 
   lua_getfield(coroutine, LUA_GLOBALSINDEX, funcName);
   lua_resume(coroutine, 0);
}

std::string ScriptEngine::getScriptPath(const std::string& scriptName)
{
   return scriptName + ".lua";
}
