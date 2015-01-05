/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef FILE_SCRIPT_H
#define FILE_SCRIPT_H

#include "Script.h"

/**
 * A FileScript is a type of Script that runs Lua code supplied in a Lua script
 * file.
 *
 * @author Noam Chitayat
 */
class FileScript : public Script
{
   public:
      /**
       * Constructor.
       * Creates a new Lua coroutine by forking the main VM.
       *
       * @param luaVM The main Lua stack to fork a coroutine from.
       * @param scriptPath The path to a script that should be run on this coroutine.
       */
      FileScript(lua_State* luaVM, const std::string& scriptPath);

      /**
       * Loads the specified script file onto the new coroutine's stack.
       *
       * @return true iff loading of the specified script was successful.
       */
      bool initialize();

      /**
       * Destructor.
       */
      ~FileScript() = default;
};

#endif
