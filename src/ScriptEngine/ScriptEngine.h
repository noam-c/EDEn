#ifndef __SCRIPT_ENGINE_H_
#define __SCRIPT_ENGINE_H_

#include <stack>
#include <string>
#include "Singleton.h"
#include "Task.h"

// We will need to talk to the tile engine from Lua
class TileEngine;
struct lua_State;
class Scheduler;

#define SCRIPT_ENG_LUA_NAME ","

/**
 * The ScriptEngine encapsulates the use of the Lua interpreter to run scripts,
 * create Lua coroutines, and bind the game functionality to the Lua scripts
 * via functions preceded by "lua" (such as luaNarrate).
 *
 * @author Noam Chitayat
 */
class ScriptEngine
{   /**
     * The scheduler for the script threads
     */
    Scheduler* scheduler;

    /**
     * The main Lua execution thread and stack
     */
    lua_State* luaVM;

    /**
     * The tile engine to execute commands on
     */
    TileEngine* tileEngine;

    /**
     * Register some functions in Lua's global space using Lua bindings
     */
    void registerFunctions();

    /**
     * Converts a script name into a relative path for the associated Lua file
     */
    std::string getScriptPath(std::string scriptName);

    public:

       /** 
        * Constructor. Initializes a Lua VM and initializes members as needed.
        *
        * @param tileEngine The tile engine to make calls to from scripts
        * @param scheduler The scheduler responsible for managing this engine's Script threads
        */
       ScriptEngine(TileEngine* tileEngine, Scheduler* scheduler);

       /**
        * Run a script with the specified name.
        *
        * @param scriptName The name of the script to run.
        */
       int runScript(std::string scriptName);

       /**
        * Run a string of script with the specified name.
        *
        * @param scriptString The name of the script to run.
        */
       int runScriptString(std::string scriptString);

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
       int narrate(lua_State* luaVM);
       int say(lua_State* luaVM);
       int playSound(lua_State* luaVM);
       int playMusic(lua_State* luaVM);
       int fadeMusic(lua_State* luaVM);
       int stopMusic(lua_State* luaVM);
       int setRegion(lua_State* luaVM);
       int addNPC(lua_State* luaVM);
       int delay(lua_State* luaVM);
};

#endif
