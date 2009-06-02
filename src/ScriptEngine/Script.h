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
{  /**
    * Runs the script until completion or yielding. Prints out any errors encountered
    * while resuming the script.
    *
    * @return true iff the script runs to completion, false if the coroutine
    *         yielded, or there was an error in execution.
    */
   bool runScript();

   protected:
      /** The stack and execution thread of this script. */
      lua_State* luaStack;

   public:
      /**
       * Constructor.
       */
      Script();

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
