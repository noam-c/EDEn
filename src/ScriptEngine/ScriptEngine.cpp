/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ScriptEngine.h"
#include "TileEngine.h"
#include "PlayerData.h"
#include "ExecutionStack.h"
#include "Scheduler.h"
#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"
#include "Timer.h"
#include "NPC.h"
#include "MovementDirection.h"
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

#define REGISTER_LUA_ENUM_VALUE(name, value) \
  lua_pushlstring(m_luaVM, #name, sizeof(#name)-1); \
  lua_pushnumber(m_luaVM, value); \
  lua_settable(m_luaVM, -3);

ScriptEngine::ScriptEngine(ExecutionStack& executionStack) :
   m_executionStack(executionStack)
{
   m_luaVM = luaL_newstate();

   if(m_luaVM == nullptr)
   {
      // An error occurred
      T_T("Unable to initialize Lua state machine");
   }

   //initialize standard Lua libraries
   luaL_openlibs(m_luaVM);

   // register game functions and enums with Lua
   registerFunctions();
   registerEnums();

   // Push this engine instance as a global pointer for the Lua VM
   // This allows global C functions in our API to reference this instance
   // (thus eliminating the need for a global ScriptEngine instance or singleton)
   lua_pushlightuserdata(m_luaVM, this);
   lua_setglobal(m_luaVM, SCRIPT_ENG_LUA_NAME);
   
   luaopen_TileEngine(m_luaVM);
   luaopen_Actor(m_luaVM);
   luaopen_PlayerCharacter(m_luaVM);
   luaopen_Quest(m_luaVM);
   luaopen_Inventory(m_luaVM);
   luaopen_Character(m_luaVM);
   luaopen_CharacterRoster(m_luaVM);
}

ScriptEngine::~ScriptEngine()
{
   if(m_luaVM)
   {
      DEBUG("Destroying Lua state machine...");
      lua_close(m_luaVM);
      DEBUG("Lua state machine destroyed.");
   }
}

void ScriptEngine::registerEnums()
{
   lua_newtable(m_luaVM);
   REGISTER_LUA_ENUM_VALUE(None, NONE);
   REGISTER_LUA_ENUM_VALUE(Up, UP);
   REGISTER_LUA_ENUM_VALUE(Down, DOWN);
   REGISTER_LUA_ENUM_VALUE(Left, LEFT);
   REGISTER_LUA_ENUM_VALUE(Right, RIGHT);
   REGISTER_LUA_ENUM_VALUE(UpLeft, UP_LEFT);
   REGISTER_LUA_ENUM_VALUE(UpRight, UP_RIGHT);
   REGISTER_LUA_ENUM_VALUE(DownLeft, DOWN_LEFT);
   REGISTER_LUA_ENUM_VALUE(DownRight, DOWN_RIGHT);
   lua_setglobal(m_luaVM, "Direction");
}

void ScriptEngine::setTileEngine(std::shared_ptr<TileEngine> engine)
{
   m_tileEngine = engine;
   if(engine)
   {
      luaW_push(m_luaVM, engine.get());
      lua_setglobal(m_luaVM, "map");

      luaW_push(m_luaVM, engine->getPlayerCharacter());
      lua_setglobal(m_luaVM, "playerSprite");
   }
   else
   {
      lua_pushnil(m_luaVM);
      lua_setglobal(m_luaVM, "map");

      lua_pushnil(m_luaVM);
      lua_setglobal(m_luaVM, "playerSprite");
   }
}

void ScriptEngine::setPlayerData(PlayerData* playerData)
{
   m_playerData = playerData;
   if(m_playerData != nullptr)
   {
      luaW_push(m_luaVM, m_playerData->getRootQuest());
      lua_setglobal(m_luaVM, "quests");

      luaW_push(m_luaVM, m_playerData->getInventory());
      lua_setglobal(m_luaVM, "inventory");

      luaW_push(m_luaVM, m_playerData->getRoster());
      lua_setglobal(m_luaVM, "roster");
   }
   else
   {
      lua_pushnil(m_luaVM);
      lua_setglobal(m_luaVM, "quests");

      lua_pushnil(m_luaVM);
      lua_setglobal(m_luaVM, "inventory");

      lua_pushnil(m_luaVM);
      lua_setglobal(m_luaVM, "roster");
   }
}
int ScriptEngine::narrate(lua_State* luaStack)
{
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if (scheduler == nullptr)
   {
      return 0;
   }
   
   auto tileEngine = m_tileEngine.lock();
   if(!tileEngine)
   {
      DEBUG("Missing tile engine. Cannot add dialogue.");
      return lua_error(luaStack);
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

   auto task = scheduler->createNewTask();
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
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if (scheduler == nullptr)
   {
      return 0;
   }

   auto tileEngine = m_tileEngine.lock();
   if(!tileEngine)
   {
      DEBUG("Missing tile engine. Cannot add dialogue.");
      return lua_error(luaStack);
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

   auto task = scheduler->createNewTask();
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
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if (scheduler == nullptr)
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

   auto task(scheduler->createNewTask());

   std::shared_ptr<Sound> sound = ResourceLoader::getSound(soundId);
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

   std::shared_ptr<Music> song = ResourceLoader::getMusic(musicId);
   song->play();

   return 0;
}

int ScriptEngine::stopMusic(lua_State* luaStack)
{
   int fadeTime;
   if(!ScriptUtilities::getParameter(luaStack, 1, -1, "fadeTime", fadeTime))
   {
      fadeTime = 0;
   }

   DEBUG("Stopping music with a fade of %d milliseconds.", fadeTime);
   Music::fadeOutMusic(fadeTime);
   return 0;
}

int ScriptEngine::isMusicPlaying(lua_State* luaStack)
{
   lua_pushboolean(luaStack, Music::isMusicPlaying());
   return 1;
}

int ScriptEngine::delay(lua_State* luaStack)
{
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if (scheduler == nullptr)
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
   
   auto waitTimer = std::make_shared<Timer>(millisecondsToWait);
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
   auto tileEngine = m_tileEngine.lock();
   if(!tileEngine)
   {
      DEBUG("Missing tile engine. Cannot set region.");
      return lua_error(luaStack);
   }

   std::string regionName;
   if(!ScriptUtilities::getParameter(luaStack, 1, 1, "name", regionName))
   {
      DEBUG("Region name not found. Cannot set region.");
      return lua_error(luaStack);
   }

   DEBUG("Setting region: %s", regionName.c_str());

   return tileEngine->setRegion(regionName);
}

std::shared_ptr<NPCScript> ScriptEngine::createNPCCoroutine(NPC* npc, const std::string& regionName, const std::string& mapName)
{
   return ScriptFactory::createNPCCoroutine(m_luaVM, npc, regionName, mapName);
}

std::shared_ptr<UsableScript> ScriptEngine::createItemScript(const Item& item) const
{
   return ScriptFactory::getItemScript(m_luaVM, item);
}

std::shared_ptr<UsableScript> ScriptEngine::createSkillScript(const Skill& skill) const
{
   return ScriptFactory::getSkillScript(m_luaVM, skill);
}

int ScriptEngine::runMapScript(const std::string& regionName, const std::string& mapName, Scheduler& scheduler)
{
   auto mapScript = ScriptFactory::getMapScript(m_luaVM, regionName, mapName);
   return runScript(mapScript, scheduler);
}

int ScriptEngine::runChapterScript(const std::string& chapterName, Scheduler& scheduler)
{
   auto chapterScript = ScriptFactory::getChapterScript(m_luaVM, chapterName);
   return runScript(chapterScript, scheduler);
}

int ScriptEngine::runScript(const std::shared_ptr<Script>& script, Scheduler& scheduler)
{
   scheduler.start(script);

   if(scheduler.hasRunningCoroutine())
   {
      return scheduler.join(script);
   }

   return 0;
}

int ScriptEngine::runScriptString(const std::string& scriptString)
{
   DEBUG("Running script string: %s", scriptString.c_str());
   auto newScript = std::make_shared<StringScript>(m_luaVM, scriptString);

   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   scheduler->start(newScript);
   
   if(scheduler->hasRunningCoroutine())
   {
      return scheduler->join(newScript);
   }
   
   return 0;
}

int ScriptEngine::runScriptFunction(void* scriptFunction)
{
   lua_pushlightuserdata(m_luaVM, scriptFunction);
   lua_pcall(m_luaVM, 0, 0, 0);
   
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
