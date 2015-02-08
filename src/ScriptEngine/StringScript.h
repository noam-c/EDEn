/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef STRING_SCRIPT_H
#define STRING_SCRIPT_H

#include "Script.h"

/**
 * A StringScript is a type of Script that runs Lua code supplied as a string of
 * instructions.
 *
 * @author Noam Chitayat
 */
class StringScript final : public Script
{
   public:
      /**
       * Constructor.
       * Creates a new Lua coroutine by forking the main VM, and then
       * loads the specified script string into the new coroutine's stack.
       *
       * @param luaVM The main Lua stack to fork a coroutine from.
       * @param scriptString The Lua code that should be run on this coroutine.
       */
      StringScript(lua_State* luaVM, const std::string& scriptString);

      /**
       * Destructor.
       */
      ~StringScript() override = default;
};

#endif
