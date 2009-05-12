#ifndef __SCRIPT_ENGINE_H_
#define __SCRIPT_ENGINE_H_

#include <stack>
#include <string>
#include "Singleton.h"
#include "TicketId.h"

// We will need to talk to the tile engine from Lua
class TileEngine;
struct lua_State;
class Scheduler;
class Script;

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
     * A stack used to keep track of which script is currently running. This
     * stack is necessary to keep track of the Script object that was last invoked,
     * so that the scripting engine can properly yield or block the thread after
     * a call from a Lua script (which only passes in a Lua stack, not a Script).
     *
     * \todo Do we need a stack? A thread should theoretically always finish or 
     * yield before any other one runs, so this stack may only have 1 entry at a
     * time anyway.
     */
    std::stack<Script*> runningScripts;

    /**
     * The current ticket ID for the next instruction
     */
    TicketId nextTicket;

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

    /**
     * Grab the next instruction ticket and queue up the next one
     *
     * @return the next instruction ticket to use.
     */
    inline TicketId getNextTicket();

    public:

       /** 
        * Constructor. Initializes a Lua VM and initializes members as needed.
        *
        * @param tileEngine The tile engine to make calls to from scripts
        * @param scheduler The scheduler responsible for managing this engine's Script threads
        */
       ScriptEngine(TileEngine* tileEngine, Scheduler* scheduler);

       /**
        * Push a script onto the runningScripts stack so the engine may refer
        * back to the script thread when needed.
        */
       void pushRunningScript(Script* script);

       /**
        * Remove the last running script from the runningScripts stack.
        */
       void popRunningScript();

       /**
        * Run a script on the main thread with the specified name.
        *
        * @param scriptName The name of the script to run.
        */
       int runScript(std::string scriptName);

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
       int setRegion(lua_State* luaVM);
};

#endif
