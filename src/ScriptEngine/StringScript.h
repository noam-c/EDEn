#ifndef __STRING_SCRIPT_H_
#define __STRING_SCRIPT_H_

#include "Script.h"

/**
 * A StringScript is a type of Script that runs Lua code supplied as a string of
 * instructions.
 *
 * @author Noam Chitayat
 */
class StringScript : public Script
{
   public:
      /**
       * Constructor.
       * Creates a new Lua thread by forking the main VM, and then
       * loads the specified script string into the new thread's stack.
       *
       * @param luaVM The main Lua stack to fork a thread from.
       * @param scriptString The Lua code that should be run on this thread.
       */
      StringScript(lua_State* luaVM, const std::string& scriptString);

      /**
       * Destructor.
       */
      ~StringScript();
};

#endif
