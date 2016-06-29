/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "ScriptEngine.h"

#include "EnumUtils.h"
#include "ExecutionStack.h"
#include "FileScript.h"
#include "Direction.h"
#include "Music.h"
#include "NPC.h"
#include "PlayerData.h"
#include "ResourceLoader.h"
#include "Scheduler.h"
#include "ScriptFactory.h"
#include "ScriptUtilities.h"
#include "Sound.h"
#include "StringScript.h"
#include "TileEngine.h"
#include "Timer.h"

#include "LuaActor.h"
#include "LuaCharacter.h"
#include "LuaCharacterRoster.h"
#include "LuaInventory.h"
#include "LuaPlayerCharacter.h"
#include "LuaQuest.h"
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
#define DEBUG_FLAG DEBUG_SCRIPT_ENG

#define REGISTER_LUA_ENUM_VALUE(name, value) \
  lua_pushlstring(m_luaVM, #name, sizeof(#name)-1); \
  lua_pushnumber(m_luaVM, value); \
  lua_settable(m_luaVM, -3);

const char* SCRIPT_ENG_LUA_NAME = ",";

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
   REGISTER_LUA_ENUM_VALUE(None, EnumUtils::toNumber(geometry::Direction::NONE));
   REGISTER_LUA_ENUM_VALUE(Up, EnumUtils::toNumber(geometry::Direction::UP));
   REGISTER_LUA_ENUM_VALUE(Down, EnumUtils::toNumber(geometry::Direction::DOWN));
   REGISTER_LUA_ENUM_VALUE(Left, EnumUtils::toNumber(geometry::Direction::LEFT));
   REGISTER_LUA_ENUM_VALUE(Right, EnumUtils::toNumber(geometry::Direction::RIGHT));
   REGISTER_LUA_ENUM_VALUE(UpLeft, EnumUtils::toNumber(geometry::Direction::UP_LEFT));
   REGISTER_LUA_ENUM_VALUE(UpRight, EnumUtils::toNumber(geometry::Direction::UP_RIGHT));
   REGISTER_LUA_ENUM_VALUE(DownLeft, EnumUtils::toNumber(geometry::Direction::DOWN_LEFT));
   REGISTER_LUA_ENUM_VALUE(DownRight, EnumUtils::toNumber(geometry::Direction::DOWN_RIGHT));
   lua_setglobal(m_luaVM, "Direction");
}

void ScriptEngine::setBattleController(std::shared_ptr<BattleController> controller)
{
   m_battleController = controller;
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

void ScriptEngine::setPlayerData(std::shared_ptr<PlayerData> playerData)
{
   m_playerData = playerData;
   if(playerData)
   {
      luaW_push(m_luaVM, playerData->getRootQuest());
      lua_setglobal(m_luaVM, "quests");

      luaW_push(m_luaVM, playerData->getInventory());
      lua_setglobal(m_luaVM, "inventory");

      luaW_push(m_luaVM, playerData->getRoster());
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

TileEngine* ScriptEngine::getRawTileEngine()
{
   auto sharedPtr = m_tileEngine.lock();
   if(sharedPtr)
   {
      return sharedPtr.get();
   }

   return nullptr;
}

int ScriptEngine::narrate(lua_State* luaStack)
{
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if (scheduler == nullptr)
   {
      return 0;
   }

   TileEngine* tileEngine = getRawTileEngine();
   if(tileEngine == nullptr)
   {
      DEBUG("Missing tile engine. Cannot add dialogue.");
      return lua_error(luaStack);
   }

   std::string text;
   ScriptUtilities::getParameter(luaStack, 1, 1, "text", text);

   std::vector<std::string> choices;
   ScriptUtilities::getParameter(luaStack, 1, 2, "choices", choices);

   if(text.empty() && choices.empty())
   {
      DEBUG("Text and choices not found. Cannot add dialogue.");
      return lua_error(luaStack);
   }

   bool waitForFinish;

   if(!choices.empty())
   {
      // Must wait for a choice to be made if choices are offered to the user
      waitForFinish = true;
   }
   else if(!ScriptUtilities::getParameter(luaStack, 1, -1, "waitForFinish", waitForFinish))
   {
      waitForFinish = false;
   }

   auto task = scheduler->createNewTask();
   DEBUG("Narrating text: %s", text.c_str());
   tileEngine->dialogueNarrate(text, task, choices);

   int numResultsExpected = choices.empty() ? 0 : 1;
   if(waitForFinish)
   {
      return waitUntilFinished(task, numResultsExpected);
   }

   return 0;
}

int ScriptEngine::say(lua_State* luaStack)
{
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if (scheduler == nullptr)
   {
      return 0;
   }

   TileEngine* tileEngine = getRawTileEngine();
   if(tileEngine == nullptr)
   {
      DEBUG("Missing tile engine. Cannot add dialogue.");
      return lua_error(luaStack);
   }

   std::string text;
   ScriptUtilities::getParameter(luaStack, 1, 1, "text", text);

   std::vector<std::string> choices;
   ScriptUtilities::getParameter(luaStack, 1, 2, "choices", choices);

   if(text.empty() && choices.empty())
   {
      DEBUG("Text and choices not found. Cannot add dialogue.");
      return lua_error(luaStack);
   }

   bool waitForFinish;

   if(!choices.empty())
   {
      // Must wait for a choice to be made if choices are offered to the user
      waitForFinish = true;
   }
   else if(!ScriptUtilities::getParameter(luaStack, 1, -1, "waitForFinish", waitForFinish))
   {
      waitForFinish = false;
   }

   auto task = scheduler->createNewTask();
   DEBUG("Saying text: %s", text.c_str());
   tileEngine->dialogueSay(text, task, choices);

   int numResultsExpected = choices.empty() ? 0 : 1;
   if(waitForFinish)
   {
      return waitUntilFinished(task, numResultsExpected);
   }

   return 0;
}

int ScriptEngine::playSound(lua_State* luaStack)
{
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if(scheduler == nullptr)
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
      return waitUntilFinished(task, 0);
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

int ScriptEngine::startBattle(lua_State* luaStack)
{
   Scheduler* scheduler = m_executionStack.getCurrentScheduler();
   if(scheduler == nullptr)
   {
      return 0;
   }

   TileEngine* tileEngine = getRawTileEngine();
      if(tileEngine == nullptr)
   {
      DEBUG("Missing tile engine. Cannot set region.");
      return lua_error(luaStack);
   }

   auto task(scheduler->createNewTask());

   DEBUG("Starting battle");
   tileEngine->startBattle(task);
   
   return waitUntilFinished(task, 0);
}

int ScriptEngine::setRegion(lua_State* luaStack)
{
   TileEngine* tileEngine = getRawTileEngine();

   if(tileEngine == nullptr)
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
   if(mapScript)
   {
      return runScript(mapScript, scheduler);
   }

   return 0;
}

int ScriptEngine::runChapterScript(const std::string& chapterName, Scheduler& scheduler)
{
   auto chapterScript = ScriptFactory::getChapterScript(m_luaVM, chapterName);
   if(chapterScript)
   {
      return runScript(chapterScript, scheduler);
   }

   return 0;
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

ScriptEngine* ScriptEngine::getScriptEngineForVM(lua_State* luaVM)
{
   lua_getglobal(luaVM, SCRIPT_ENG_LUA_NAME);
   ScriptEngine* engine = static_cast<ScriptEngine*>(lua_touserdata(luaVM, lua_gettop(luaVM)));
   lua_pop(luaVM, 1);
   return engine;
}

std::shared_ptr<Task> ScriptEngine::createTask()
{
   auto scheduler = m_executionStack.getCurrentScheduler();
   return scheduler->createNewTask();
}

int ScriptEngine::waitUntilFinished(const std::shared_ptr<Task>& pendingTask, int numResults)
{
   auto scheduler = m_executionStack.getCurrentScheduler();
   return scheduler->block(pendingTask, numResults);
}

void ScriptEngine::callFunction(lua_State* coroutine, const char* funcName)
{
   // push the function onto the stack and then resume the coroutine from the
   // start of the function
   lua_getglobal(coroutine, funcName);
   lua_resume(coroutine, nullptr, 0);
}

std::string ScriptEngine::getScriptPath(const std::string& scriptName)
{
   return scriptName + ".lua";
}
