#ifndef __SCRIPT_ENGINE_H_
#define __SCRIPT_ENGINE_H_

#include <stack>
#include <string>
#include "Singleton.h"
#include "TicketId.h"

// We will need to talk to the tile engine from Lua
class TileEngine;
struct lua_State;
class ScriptScheduler;
class Script;

/**
 * The Script Engine encapsulates the use of the Lua interpreter to run scripts,
 * create Lua coroutines, and bind the game functionality to the Lua scripts
 * via functions preceded by "lua" (such as luaNarrate).
 *
 * This class is a singleton.
 *
 * @author Noam Chitayat
 */
class ScriptEngine : public Singleton<ScriptEngine>
{   /**
     * The scheduler for the script threads
     */
    ScriptScheduler* scheduler;

    /**
     * \todo Document.
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

    protected:

       /**
        * Singleton constructor. Private since the engine is a singleton.
        */
       void initialize() throw();

       /**
        * Singleton destructor.
        */
       void finish();

    public:

       /**
        * \todo Document.
        */
       void pushRunningScript(Script* script);

       /**
        * \todo Document.
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
        * \todo Document.
        */
       void runThreads(long timePassed);

       /**
        * \todo Document.
        */
       void signalTicket(TicketId ticket);

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

       /////////////////////////////////////////////////////////
       /////////// Functions supplied to Lua scripts ///////////
       /////////////////////////////////////////////////////////
       int narrate(lua_State* luaVM);
       int say(lua_State* luaVM);
       int setRegion(lua_State* luaVM);
};

#endif
