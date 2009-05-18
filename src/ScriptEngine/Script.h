#ifndef __SCRIPT_H_
#define __SCRIPT_H_

#include "Thread.h"
#include <string>

struct lua_State;

/**
 * A Script is a type of Thread that runs a Lua coroutine. As such, the Script
 * object can resume or yield a Lua thread of execution.
 *
 * @author Noam Chitayat
 */
class Script : public Thread
{  /** The stack and execution thread of this script. */
   lua_State* luaStack;

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
       * Constructor.
       */
      Script();

      /**
       * Creates a new Lua thread by forking the main VM, and then
       * loads the specified script file onto the new thread's stack.
       *
       * @param luaVM The main Lua stack to fork a thread from.
       * @param scriptPath The path to a script that should be run on this thread.
       */
      void loadFile(lua_State* luaVM, std::string scriptPath);

      /**
       * Creates a new Lua thread by forking the main VM, and then
       * loads the specified script string into the new thread's stack.
       *
       * @param luaVM The main Lua stack to fork a thread from.
       * @param scriptString The Lua code that should be run on this thread.
       */
      void loadString(lua_State* luaVM, std::string scriptString);

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
