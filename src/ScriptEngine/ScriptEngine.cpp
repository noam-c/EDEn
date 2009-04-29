#include "ScriptEngine.h"
#include "TileEngine.h"
#include "ScriptScheduler.h"

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

int ScriptEngine::narrate(lua_State* luaVM)
{  int nargs = lua_gettop(luaVM);
   bool waitForFinish = false;

   if(nargs > 0)
   {  const char* speech = lua_tostring(luaVM, 1);
      if(nargs == 2)
      {  waitForFinish = lua_toboolean(luaVM, 2);
      }

      TicketId ticket = getNextTicket();

      DEBUG2("Narrating text: ", speech);
      tileEngine->dialogueNarrate(speech, ticket);

      if(waitForFinish)
      {  scheduler->block(luaVM, ticket);
         return lua_yield(luaVM, 0);
      }
   }

   return 0;
}

int ScriptEngine::say(lua_State* luaVM)
{  int nargs = lua_gettop(luaVM);
   bool waitForFinish = false;

   if(nargs > 0)
   {  const char* speech = lua_tostring(luaVM, 1);
      if(nargs == 2)
      {  waitForFinish = lua_toboolean(luaVM, 2);
      }

      TicketId ticket = getNextTicket();

      DEBUG2("Saying text: ", speech);
      tileEngine->dialogueSay(speech, ticket);

      if(waitForFinish)
      {  scheduler->block(luaVM, ticket);
         return lua_yield(luaVM, 0);
      }
   }

   return 0;
}

int ScriptEngine::setRegion(lua_State* luaVM)
{  int nargs = lua_gettop(luaVM);
   if(nargs > 0)
   {  std::string regionName(lua_tostring(luaVM, 1));
      if(nargs == 2)
      {  bool waitForFinish = lua_toboolean(luaVM, 2);
         if(waitForFinish)
         {  /** \todo We need to handle the wait case, not just yield blindly.
             */
            return lua_yield(luaVM, 0);
         } 
      }

      DEBUG2("Setting region: ", regionName);
      
      return tileEngine->setRegion(luaVM, regionName);
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

int ScriptEngine::runScript(std::string scriptName, lua_State* state)
{  DEBUG2("Running script: ", scriptName);
   lua_State* newState = makeThread(scriptName);
   scheduler->start(newState);
   if(state != NULL)
   {  scheduler->join(state, newState);
      DEBUG("Yielding: ");
      DEBUG((int)state);
      return lua_yield(state, 0);
   }

   return 0;
}

void ScriptEngine::signalTicket(TicketId ticket)
{  scheduler->ready(ticket);
}

bool ScriptEngine::resumeScript(lua_State* state)
{  int returnCode = lua_resume(state, 0);
   if(returnCode == 0)
   {  DEBUG("Closing state.");
      DEBUG((int)state);
      //lua_close(state); // Do we need to close each thread, or just the main one?
      return true;
   }
   else if(returnCode == LUA_YIELD)
   {  DEBUG("State yielded.");
      return false;
   }

   // An error occurred: Print out the error message
   DEBUG2("Error running script: ", lua_tostring(state, -1));

   return false;
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

lua_State* ScriptEngine::makeThread(std::string scriptName)
{  lua_State* newThread = lua_newthread(luaVM);
   luaL_loadfile(newThread, getScriptPath(scriptName).c_str());
   return newThread;
}

void ScriptEngine::runThreads()
{  scheduler->run();
}

std::string ScriptEngine::getScriptPath(std::string scriptName)
{  return scriptName + ".lua";
}
