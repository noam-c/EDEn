/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
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
#include "ScriptUtilities.h"

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

   std::string text;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "text", text))
   {
      DEBUG("Text not found. Cannot add dialogue.");
      return lua_error(luaStack);
   }

   bool waitForFinish;
   if(!ScriptUtilities::getParameter(luaStack, 1, -1, "waitForFinish", waitForFinish))
   {
      waitForFinish = false;
   }

   int callResult = 0;

   Task* task = Task::getNextTask(*scheduler);
   if(waitForFinish)
   {
      callResult = scheduler->block(task);
   }

   DEBUG("Narrating text: %s", text.c_str());
   tileEngine->dialogueNarrate(text, task);

   return callResult;
}

int ScriptEngine::say(lua_State* luaStack)
{
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   if (scheduler == NULL)
   {
      return 0;
   }

   std::string text;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "text", text))
   {
      DEBUG("Text not found. Cannot add dialogue.");
      return lua_error(luaStack);
   }

   bool waitForFinish;
   if(!ScriptUtilities::getParameter(luaStack, 1, -1, "waitForFinish", waitForFinish))
   {
      waitForFinish = false;
   }

   int callResult = 0;

   Task* task = Task::getNextTask(*scheduler);
   if(waitForFinish)
   {
      callResult = scheduler->block(task);
   }

   DEBUG("Saying text: %s", text.c_str());
   tileEngine->dialogueSay(text, task);

   return callResult;
}

int ScriptEngine::playSound(lua_State* luaStack)
{
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   if (scheduler == NULL)
   {
      return 0;
   }

   std::string soundId;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "id", soundId))
   {
      DEBUG("Sound ID not found. Cannot play sound.");
      return lua_error(luaStack);
   }

   bool waitForFinish;
   if(!ScriptUtilities::getParameter(luaStack, 1, -1, "waitForFinish", waitForFinish))
   {
      waitForFinish = false;
   }

   DEBUG("Playing sound: %s", soundId.c_str());

   Task* task = Task::getNextTask(*scheduler);

   Sound* sound = ResourceLoader::getSound(soundId);
   sound->play(task);

   if(waitForFinish)
   {
      return scheduler->block(task);
   }

   return 0;
}

int ScriptEngine::playMusic(lua_State* luaStack)
{
   std::string musicId;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "id", musicId))
   {
      DEBUG("Music ID not found. Cannot play sound.");
      return lua_error(luaStack);
   }

   DEBUG("Playing music: %s", musicId.c_str());

   Music* song = ResourceLoader::getMusic(musicId);
   song->play();

   return 0;
}

int ScriptEngine::stopMusic(lua_State* luaStack)
{
   DEBUG("Stopping music.");
   Music::stopMusic();
   return 0;
}

int ScriptEngine::delay(lua_State* luaStack)
{
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   if (scheduler == NULL)
   {
      return 0;
   }

   long millisecondsToWait;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "ms", millisecondsToWait))
   {
      DEBUG("Delay duration not found. Cannot create timed delay.");
      return lua_error(luaStack);
   }

   DEBUG("Waiting %d milliseconds", millisecondsToWait);
   
   Timer* waitTimer = new Timer(millisecondsToWait);
   scheduler->start(waitTimer);
   
   return scheduler->join(waitTimer);
}

int ScriptEngine::generateRandom(lua_State* luaStack)
{
   long minValue;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "min", minValue))
   {
      DEBUG("Minimum value not found. Cannot generate random number.");
      return lua_error(luaStack);
   }

   long maxValue;
   if(!ScriptUtilities::getParameter(luaStack, 1, 2, "max", maxValue))
   {
      DEBUG("Maximum value not found. Cannot generate random number.");
      return lua_error(luaStack);
   }

   if(maxValue < minValue)
   {
      DEBUG("Cannot generate random number: Maximum value is smaller than minimum value.");
      return lua_error(luaStack);
   }
   
   /** \todo Random number generation is currently unseeded. Use a good seed. */
   lua_pushnumber(luaStack, (rand() % (maxValue - minValue)) + minValue);

   return 1;
}

int ScriptEngine::setRegion(lua_State* luaStack)
{
   std::string regionName;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "name", regionName))
   {
      DEBUG("Region name not found. Cannot set region.");
      return lua_error(luaStack);
   }

   DEBUG("Setting region: %s", regionName.c_str());

   return tileEngine->setRegion(regionName);
}

NPCScript* ScriptEngine::createNPCCoroutine(NPC* npc, const std::string& regionName, const std::string& mapName)
{
   return ScriptFactory::createNPCCoroutine(luaVM, npc, regionName, mapName);
}

UsableScript* ScriptEngine::createItemScript(const Item& item) const
{
   return ScriptFactory::getItemScript(luaVM, item);
}

UsableScript* ScriptEngine::createSkillScript(const Skill& skill) const
{
   return ScriptFactory::getSkillScript(luaVM, skill);
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
