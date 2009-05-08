#include "ScriptEngine.h"
#include "TileEngine.h"
#include "ScriptScheduler.h"
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

void ScriptEngine::initialize() throw()
{  luaVM = luaL_newstate();

   if(luaVM == NULL)
   {  // An error occurred
      T_T("Unable to initialize Lua state machine");
   }

   //initialize standard Lua libraries
   luaL_openlibs(luaVM);

   // register game functions with Lua
   registerFunctions();

   // initialize the script scheduler
   scheduler = new ScriptScheduler();

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

      Script* currScript = runningScripts.top();
      if(waitForFinish)
      {  scheduler->block(currScript, ticket);
         return currScript->yield();
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

      Script* currScript = runningScripts.top();
      if(waitForFinish)
      {  scheduler->block(currScript, ticket);
         return currScript->yield();
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

void ScriptEngine::finish()
{  if(luaVM)
   {  DEBUG("Destroying Lua state machine...");
      lua_close(luaVM);
   }

   delete scheduler;
}

int ScriptEngine::runScript(std::string scriptName)
{  DEBUG("Running script: %s", scriptName.c_str());
   Script* newScript = new Script(luaVM, getScriptPath(scriptName));
   scheduler->start(newScript);

   if(!runningScripts.empty())
   {  Script* currScript = runningScripts.top();
      scheduler->join(currScript, newScript);
      DEBUG("Yielding: %d", currScript->getId());
      return currScript->yield();
   }

   return 0;
}

void ScriptEngine::signalTicket(TicketId ticket)
{  scheduler->ready(ticket);
}

void ScriptEngine::setTileEngine(TileEngine* engine)
{  tileEngine = engine;
}

void ScriptEngine::callFunction(lua_State* thread, const char* funcName)
{  // push the function onto the stack and then resume the thread from the
   // start of the function 
   lua_getfield(thread, LUA_GLOBALSINDEX, funcName);
   lua_resume(thread, 0);
}

void ScriptEngine::pushRunningScript(Script* script)
{  runningScripts.push(script);
}

void ScriptEngine::popRunningScript()
{  runningScripts.pop();
}

void ScriptEngine::runThreads(long timePassed)
{  scheduler->run(timePassed);
}

std::string ScriptEngine::getScriptPath(std::string scriptName)
{  return scriptName + ".lua";
}
