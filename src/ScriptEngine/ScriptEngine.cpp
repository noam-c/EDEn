#include "ScriptEngine.h"
#include "TileEngine.h"
#include "Scheduler.h"
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

ScriptEngine::ScriptEngine(TileEngine* tileEngine, Scheduler* scheduler)
                                  : tileEngine(tileEngine), scheduler(scheduler)
{  luaVM = luaL_newstate();

   if(luaVM == NULL)
   {  // An error occurred
      T_T("Unable to initialize Lua state machine");
   }

   //initialize standard Lua libraries
   luaL_openlibs(luaVM);

   // register game functions with Lua
   registerFunctions();

   // Push this engine instance as a global pointer for the Lua VM
   // This allows global C functions in our API to reference this instance
   // (thus eliminating the need for a global ScriptEngine instance or singleton)
   lua_pushlightuserdata(luaVM, this);
   lua_setglobal(luaVM, SCRIPT_ENG_LUA_NAME);

   nextTicket = 0;
}

TicketId ScriptEngine::getNextTicket()
{  return nextTicket++;
}

int ScriptEngine::narrate(lua_State* luaStack)
{  int nargs = lua_gettop(luaStack);
   bool waitForFinish = false;

   if(nargs > 0)
   {  const char* speech = lua_tostring(luaStack, 1);
      if(nargs == 2)
      {  waitForFinish = lua_toboolean(luaStack, 2);
      }

      TicketId ticket = getNextTicket();

      DEBUG("Narrating text: %s", speech);
      tileEngine->dialogueNarrate(speech, ticket);

      if(waitForFinish)
      {  return scheduler->block(ticket);;
      }
   }

   return 0;
}

int ScriptEngine::say(lua_State* luaStack)
{  int nargs = lua_gettop(luaStack);
   bool waitForFinish = false;

   if(nargs > 0)
   {  const char* speech = lua_tostring(luaStack, 1);
      if(nargs == 2)
      {  waitForFinish = lua_toboolean(luaStack, 2);
      }

      TicketId ticket = getNextTicket();

      DEBUG("Saying text: %s", speech);
      tileEngine->dialogueSay(speech, ticket);

      if(waitForFinish)
      {  return scheduler->block(ticket);
      }
   }

   return 0;
}

int ScriptEngine::setRegion(lua_State* luaStack)
{  int nargs = lua_gettop(luaStack);
   if(nargs > 0)
   {  std::string regionName(lua_tostring(luaStack, 1));
      DEBUG("Setting region: %s", regionName.c_str());

      std::string mapScriptPath = tileEngine->setRegion(regionName);
      return runScript(mapScriptPath);
   }

   return 0;
}

int ScriptEngine::runScript(std::string scriptName)
{  DEBUG("Running script: %s", scriptName.c_str());
   Script* newScript = new Script(luaVM, getScriptPath(scriptName));
   scheduler->start(newScript);

   if(scheduler->hasRunningThread())
   {  return scheduler->join(newScript);
   }

   return 0;
}

void ScriptEngine::callFunction(lua_State* thread, const char* funcName)
{  // push the function onto the stack and then resume the thread from the
   // start of the function 
   lua_getfield(thread, LUA_GLOBALSINDEX, funcName);
   lua_resume(thread, 0);
}

std::string ScriptEngine::getScriptPath(std::string scriptName)
{  return scriptName + ".lua";
}

ScriptEngine::~ScriptEngine()
{  if(luaVM)
   {  DEBUG("Destroying Lua state machine...");
      lua_close(luaVM);
   }
}
