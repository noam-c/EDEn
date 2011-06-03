#ifndef SCRIPT_H
#define SCRIPT_H

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
{
   protected:
      /** The stack and execution thread of this script. */
      lua_State* luaStack;

      /** The name of the script. */
      std::string scriptName;

      /** Flag indicating whether or not the script is currently in the middle of a run */
      bool running;

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
      Script(const std::string& name);

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

      /**
       * Destructor. Made abstract in order to make Scripts abstract.
       */
      virtual ~Script() = 0;
};

#endif
