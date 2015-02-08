/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef SCRIPT_H
#define SCRIPT_H

#include "Coroutine.h"
#include <string>

struct lua_State;

/**
 * A Script is a type of Coroutine that runs a Lua coroutine. As such, the Script
 * object can resume or yield a Lua coroutine of execution.
 *
 * @author Noam Chitayat
 */
class Script : public Coroutine
{
   protected:
      /** The stack and execution coroutine of this script. */
      lua_State* m_luaStack;

      /** The name of the script. */
      std::string m_scriptName;

      /** Flag indicating whether or not the script is currently in the middle of a run */
      bool m_running;

      /**
       * Runs the script until completion or yielding. Prints out any errors encountered
       * while resuming the script.
       *
       * @param numArgs The number of arguments passed to the script.
       *
       * @return true iff the script runs to completion, false if the coroutine
       *         yielded, or there was an error in execution.
       */
      bool runScript(int numArgs = 0);

   public:
      /**
       * Constructor.
       */
      Script(const std::string& name);

      /**
       * Performs a Lua resume on the coroutine.
       * @return true iff the script runs to completion, false if the coroutine
       *         yielded, or there was an error in execution.
       */
      virtual bool resume(long timePassed) override;

      /**
       * Yield the script execution
       *
       * @param The number of results to be returned to the script when execution resumes.
       *
       * @return The Lua yield code (LUA_YIELD) so that the resumer of the coroutine
       *         can regain control and handle the yield.
       */
      int yield(int numResults) override;

      /**
       * Destructor. Made abstract in order to make Scripts abstract.
       */
      virtual ~Script() override = 0;
};

#endif
