#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Thread.h"
#include <string>

struct lua_State;
class ScriptEngine;

/**
 * A Script is a type of Thread that runs a Lua coroutine. As such, the Script
 * object can resume or yield a Lua thread of execution.
 *
 * @author Noam Chitayat
 */
class Script : public Thread
{  /** The stack and execution thread of this script. */
   lua_State* luaStack;

   /** The ScriptEngine that invokes this script */
   ScriptEngine* engine;

   /**
    * Runs the script until completion or yielding. Prints out any errors encountered
    * while resuming the script.
    *
    * @return true iff the script runs to completion, false if the coroutine
    *         yielded, or there was an error in execution.
    */
   bool runScript();

   public:
      /**
       * Constructor. Creates a new Lua thread by forking the main VM, and then
       * loads the specified script onto the new thread's stack.
       *
       * @param scriptEngine The scripting engine that will invoke this thread
       * @param luaVM The main Lua stack to fork a thread from.
       * @param scriptPath The path to a script that should be run on this thread.
       */
      Script(ScriptEngine* scriptEngine, lua_State* luaVM, std::string scriptPath);

      /**
       * Performs a Lua resume on the thread.
       * @return true iff the script runs to completion, false if the coroutine
       *         yielded, or there was an error in execution.
       */
      bool resume(long timePassed);

      /**
       * Yield the thread
       *
       * @return The Lua yield code (LUA_YIELD) so that the resumer of the thread
       *         can regain control and handle the yield.
       */
      int yield();
};

#endif
