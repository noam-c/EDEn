#ifndef __SCRIPT_ENGINE_H_
#define __SCRIPT_ENGINE_H_

#include <string>

// We will need to talk to the tile engine from Lua
class TileEngine;
struct lua_State;

/**
 * The Script Engine encapsulates the use of the Lua interpreter to run scripts,
 * create Lua coroutines, and bind the game functionality to the Lua scripts
 * via functions preceded by "lua" (such as luaNarrate).
 *
 * @author Noam Chitayat
 */
class ScriptEngine
{   /**
     * The singleton instance.
     */
    static ScriptEngine* instance;

    /**
     * The main Lua execution thread and stack
     */
    lua_State* luaVM;

    /**
     * The tile engine to execute commands on
     */
    TileEngine* tileEngine;

    /**
     * Constructor. Private since the engine is a singleton.
     */
    ScriptEngine() throw();

    /**
     * Register some functions in Lua's global space using Lua bindings
     */
    void registerFunctions();

    /**
     * Converts a script name into a relative path for the associated Lua file
     */
    std::string getScriptPath(std::string scriptName);

    /**
     * Destructor.
     */
    ~ScriptEngine();

    public:

       /**
        * @return The singleton instance of the script engine.
        */
       static ScriptEngine* getInstance() throw();

       /**
        * Run a script on the main thread with the specified name.
        *
        * @param scriptName The name of the script to run.
        */
       void runScript(std::string scriptName);

       /**
        * Set the tile engine to send commands to.
        *
        * @param engine The tile engine to set.
        */
       void setTileEngine(TileEngine* engine);

       /**
        * Create a new thread with a specified script loaded in.
        *
        * @param scriptName The name of the script to run.
        *
        * @return Pointer to a new Lua coroutine and its execution stack
        */
       lua_State* makeThread(std::string scriptName);

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
        * Destroys the singleton instance.
        */
       static void destroy();

       int narrate(lua_State* luaVM);
       int say(lua_State* luaVM);
       int setRegion(lua_State* luaVM);
};

#endif
