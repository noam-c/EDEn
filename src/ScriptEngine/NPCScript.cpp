#include "NPCScript.h"
#include "NPC.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

const char* NPCScript::FUNCTION_NAMES[] = { "idle" };

NPCScript::NPCScript(lua_State* luaVM, std::string scriptPath, NPC* npc) : Script(scriptPath), npc(npc), finished(false)
{  luaStack = lua_newthread(luaVM);
   DEBUG("Script ID %d loading functions from %s", getId(), scriptPath.c_str());
   luaL_dofile(luaStack, scriptPath.c_str());

   // All the below code simply takes all the global functions that the script
   // created, and pushes them into a table that uses this NPC's unique
   // identifier. Then it removes those global functions for safety.

   // Sure, this would be much easier to do in the Lua script itself, but
   // I want to have absolutely no boilerplate code or metatable BS inside the
   // Lua scripts themselves; I want the .lua files to all be approachable by
   // novice programmers and even non-programmers, and I don't want them to see
   // "black box" code in each file. This should never cause a problem for the
   // flexibility of the scripts, because anything you can do in Lua, you can
   // do using the Lua/C++ API.

   // Create a table for this NPC's functions
   lua_createtable(luaStack, 0, NUM_FUNCTIONS);

   for(int i = 0; i < NUM_FUNCTIONS; ++i)
   {  DEBUG("Checking for function %s.", FUNCTION_NAMES[i]);

      // Push the function name into the table
      lua_pushstring(luaStack, FUNCTION_NAMES[i]);

      // Push the value of "idle" onto the stack
      lua_getglobal(luaStack, FUNCTION_NAMES[i]);

      // Do a type-check here and push an empty function if needed
      if(!lua_isfunction(luaStack, -1))
      {  DEBUG("%s was not found to be a function!", FUNCTION_NAMES[i]);
         /**
          * \todo Create a Lua constant empty function somewhere, and use it
          * here.
          */
         // Pop off the bad function
         //lua_pop(luaStack, 1);

         // Push on the empty function
         //lua_getglobal(luaStack, "emptyFunction");
      }

      // Grabs the table explicitly, and uses the string function name as a key
      // to push in the global function we found (pops string and function off the stack)
      lua_rawset(luaStack, -3);

      // Remove the function from the global table so nobody else accidentally runs into it
      lua_pushnil(luaStack);
      lua_setglobal(luaStack, FUNCTION_NAMES[i]);
   }

   // Push the table into the global space with the file path as the name
   lua_setglobal(luaStack, scriptName.c_str());
}

bool NPCScript::callFunction(std::string functionName)
{  // Load the table of Lua functions for this NPC
	lua_getglobal(luaStack, scriptName.c_str());

	// Get the function 'functionName' from this table
   lua_pushstring(luaStack, functionName.c_str());
   lua_rawget(luaStack, -2);

   // Run the script
	return runScript();
}

bool NPCScript::resume(long timePassed)
{  if(finished) return true;
   if(running)
   {  DEBUG("NPC Thread %d resuming running script.", getId());
      runScript();
   }
   else
   {  if(npc->isIdle())
      {  DEBUG("NPC Thread %d idling.", getId());
         callFunction("idle");
      }
   }

   return false;
}

void NPCScript::activate()
{  callFunction("activate");
}

void NPCScript::finish()
{  finished = true;
}

NPCScript::~NPCScript()
{  /** \todo Check if this cleanup is appropriate. */
   // Set the function table to nil so that it gets garbage collected
   // lua_pushnil(luaStack);
   // lua_setglobal(luaStack, scriptPath.c_str());
}
