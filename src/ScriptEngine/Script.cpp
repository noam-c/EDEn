#include "Script.h"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

#include "DebugUtils.h"
const int debugFlag = DEBUG_SCRIPT_ENG;

Script::Script(lua_State* luaVM, std::string scriptPath)
{  luaStack = lua_newthread(luaVM);
   luaL_loadfile(luaStack, scriptPath.c_str());
}

bool Script::runScript()
{  DEBUG("Resuming script %d...", threadId);

   int returnCode = lua_resume(luaStack, 0);
   if(returnCode == 0)
   {  DEBUG("Script %d finished.", threadId);
      return true;
   }
   else if(returnCode == LUA_YIELD)
   {  DEBUG("Script %d yielded.", threadId);
      return false;
   }

   // An error occurred: Print out the error message
   DEBUG("Error running script: %s", lua_tostring(luaStack, -1));

   return false;
}

bool Script::resume(long /*timePassed*/)
{  bool result = runScript();
   return result;
}

int Script::yield()
{  return lua_yield(luaStack, 0);
}
