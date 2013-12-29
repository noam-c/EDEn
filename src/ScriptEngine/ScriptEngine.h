/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SCRIPT_ENGINE_H
#define SCRIPT_ENGINE_H

#include <stack>
#include <string>
#include "Singleton.h"
#include "Task.h"

// We will need to talk to the tile engine and player data from Lua
class TileEngine;
class PlayerData;
class ExecutionStack;

class Scheduler;
class Script;
class NPC;
class NPCScript;
typedef class Usable Item;
class Skill;
class UsableScript;

struct lua_State;

#define SCRIPT_ENG_LUA_NAME ","

/**
 * The ScriptEngine encapsulates the use of the Lua interpreter to run scripts,
 * create Lua coroutines, and bind the game functionality to the Lua scripts
 * via functions preceded by "lua" (such as luaNarrate).
 *
 * @author Noam Chitayat
 */
class ScriptEngine
{
   /**
    * The tile engine to execute commands on
    */
   TileEngine* m_tileEngine;
   
   /**
    * The player data to execute commands on
    */
   PlayerData* m_playerData;

   /**
    * The stack managing the current game state.
    */
   ExecutionStack& m_executionStack;

   /**
    * The main Lua execution coroutine and stack
    */
   lua_State* m_luaVM;

   /**
    * Register some functions in Lua's global space using Lua bindings
    */
   void registerFunctions();

   /**
    * Register some enums in Lua's global space using Lua bindings
    */
   void registerEnums();

   /**
    * Converts a script name into a relative path for the associated Lua file
    */
   std::string getScriptPath(const std::string& scriptName);

   /**
    * Run a specified script.
    *
    * @param script The script to run.
    * @param scheduler The scheduler that will manage the new script coroutine.
    */
   int runScript(Script* script, Scheduler& scheduler);

   public:
      /** 
       * Constructor. Initializes a Lua VM and initializes members as needed.
       *
       * @param executionStack The stack containing the current game state.
       */
      ScriptEngine(ExecutionStack& executionStack);

      /**
       * Get a specified NPC script.
       *
       * @param npc The NPC to bind to the requested script.
       * @param regionName The name of the region this NPC is found in.
       * @param mapName The name of the map this NPC is found in.
       */
      NPCScript* createNPCCoroutine(NPC* npc, const std::string& regionName, const std::string& mapName);

      /**
       * Get a specified item's script.
       *
       * @param item The item to load the script for.
       */
      UsableScript* createItemScript(const Item& item) const;

      /**
       * Get a specified skill's script.
       *
       * @param skill The skill to load the script for.
       */
      UsableScript* createSkillScript(const Skill& skill) const;

      /**
       * Run a specified map script.
       *
       * @param regionName The region containing the map script to run.
       * @param mapName The name of the map script to be run.
       * @param scheduler The scheduler that will manage the new script coroutine.
       */
      int runMapScript(const std::string& regionName, const std::string& mapName, Scheduler& scheduler);

      /**
       * Run a specified chapter script.
       *
       * @param chapterName The name of the chapter script to run.
       * @param scheduler The scheduler that will manage the new script coroutine.
       */
      int runChapterScript(const std::string& chapterName, Scheduler& scheduler);
      
      /**
       * Run a string of script with the specified name.
       *
       * @param scriptString The name of the script to run.
       */
      int runScriptString(const std::string& scriptString);
      
      /**
       * Call a script function object.
       *
       * @param scriptFunction The raw script function to run.
       */
      int runScriptFunction(void* scriptFunction);

      /**
       * Set the tile engine to send commands to.
       *
       * @param engine The tile engine to set.
       */
      void setTileEngine(TileEngine* engine);

      /**
       * Set the player data to send commands to.
       *
       * @param playerData The player data that the scripts will reference
       */
      void setPlayerData(PlayerData* playerData);

      /**
       * Calls a specified function on a specified Lua coroutine.
       * Because of the nature of Lua's argument pushing, passing an array of
       * arguments here will be complicated and may not be worth adding in.
       *
       * \todo We should add error handling in case the function doesn't exist
       * in this coroutine.
       *
       * @param coroutine The coroutine to run the function on. This can be the main
       * coroutine.
       * @param funcName The name of the function to call.
       */
      void callFunction(lua_State* coroutine, const char* funcName);

      /** 
       * Destructor. Cleans up used memory and closes the Lua VM,
       * disposing of any memory used by the Lua scripts.
       */
      ~ScriptEngine();

      /////////////////////////////////////////////////////////
      /////////// Functions supplied to Lua scripts ///////////
      /////////////////////////////////////////////////////////
      int narrate(lua_State* luaStack);
      int say(lua_State* luaStack);
      int playSound(lua_State* luaStack);
      int playMusic(lua_State* luaStack);
      int stopMusic(lua_State* luaStack);
      int isMusicPlaying(lua_State* luaStack);
      int delay(lua_State* luaStack);
      int generateRandom(lua_State* luaStack);

      ///////////////// Tile engine functions /////////////////
      int setRegion(lua_State* luaStack);
};

#endif
