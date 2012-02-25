/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
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

class Scheduler;
class NPC;
class Script;
class NPCScript;

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
   TileEngine& tileEngine;
   
   /**
    * The player data to execute commands on
    */
   PlayerData& playerData;

   /**
    * The scheduler for the script threads
    */
   Scheduler& scheduler;

   /**
    * The main Lua execution thread and stack
    */
   lua_State* luaVM;

   /**
    * Register some functions in Lua's global space using Lua bindings
    */
   void registerFunctions();

   /**
    * Converts a script name into a relative path for the associated Lua file
    */
   std::string getScriptPath(const std::string& scriptName);

   /**
    * Run a specified script.
    *
    * @param script The script to run.
    */
   int runScript(Script* script);

   public:
      /** 
       * Constructor. Initializes a Lua VM and initializes members as needed.
       *
       * @param tileEngine The tile engine to make calls to from scripts
       * @param playerData The player data that the scripts will reference
       * @param scheduler The scheduler responsible for managing this engine's Script threads
       */
      ScriptEngine(TileEngine& tileEngine, PlayerData& playerData, Scheduler& scheduler);

      /**
       * Get a specified NPC script.
       *
       * @param npc The NPC to bind to the requested script.
       * @param regionName The name of the region this NPC is found in.
       * @param mapName The name of the map this NPC is found in.
       * @param npcName The name of the NPC (and its script file).
       */
      NPCScript* getNPCScript(NPC* npc, const std::string& regionName, const std::string& mapName, const std::string& npcName);

      /**
       * Run a specified map script.
       *
       * @param regionName The region containing the map script to run.
       * @param mapName The name of the map script to be run.
       */
      int runMapScript(const std::string& regionName, const std::string& mapName);

      /**
       * Run a specified chapter script.
       *
       * @param chapterName The name of the chapter script to run.
       */
      int runChapterScript(const std::string& chapterName);

      /**
       * Run a string of script with the specified name.
       *
       * @param scriptString The name of the script to run.
       */
      int runScriptString(const std::string& scriptString);

      /**
       * Set the tile engine to send commands to.
       *
       * @param engine The tile engine to set.
       */
      void setTileEngine(TileEngine* engine);

      /**
       * Calls a specified function on a specified Lua thread.
       * Because of the nature of Lua's argument pushing, passing an array of
       * arguments here will be complicated and may not be worth adding in.
       *
       * \todo We should add error handling in case the function doesn't exist
       * in this thread.
       *
       * @param thread The thread to run the function on. This can be the main
       * thread.
       * @param funcName The name of the function to call.
       */
      void callFunction(lua_State* thread, const char* funcName);

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
      int fadeMusic(lua_State* luaStack);
      int stopMusic(lua_State* luaStack);
      int delay(lua_State* luaStack);
      int generateRandom(lua_State* luaStack);

      ///////////////// Tile engine functions /////////////////
      int setRegion(lua_State* luaStack);
};

#endif
