#include "ScriptEngine.h"
#include "TileEngine.h"

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

ScriptEngine* ScriptEngine::instance = NULL;

ScriptEngine::ScriptEngine() throw()
{  luaVM = luaL_newstate();

   if(luaVM == NULL)
   {  // An error occurred
      T_T("Unable to initialize Lua state machine");
   }

   //initialize standard Lua libraries
   luaL_openlibs(luaVM);

   // register game functions with Lua
   registerFunctions();
}

int ScriptEngine::narrate(lua_State* luaVM)
{  int nargs = lua_gettop(luaVM);
   if(nargs > 0)
   {  const char* speech = lua_tostring(luaVM, -1);
      if(nargs == 2)
      {  bool waitForFinish = lua_toboolean(luaVM, 2);
         if(waitForFinish)
         {  /** \todo We need to handle the wait case, not just yield blindly.
             */
            return lua_yield(luaVM, 0);
         } 
      }

      DEBUG2("Narrating text: ", speech);
      tileEngine->dialogueNarrate(speech);
   }

   return 0;
}

int ScriptEngine::say(lua_State* luaVM)
{  int nargs = lua_gettop(luaVM);
   if(nargs > 0)
   {  const char* speech = lua_tostring(luaVM, -1);
      if(nargs == 2)
      {  bool waitForFinish = lua_toboolean(luaVM, 2);
         if(waitForFinish)
         {  /** \todo We need to handle the wait case, not just yield blindly.
             */
            return lua_yield(luaVM, 0);
         } 
      }

      DEBUG2("Saying text: ", speech);
      tileEngine->dialogueSay(speech);
   }

   return 0;
}

int ScriptEngine::setRegion(lua_State* luaVM)
{  int nargs = lua_gettop(luaVM);
   if(nargs > 0)
   {  std::string regionName(lua_tostring(luaVM, -1));
      if(nargs == 2)
      {  bool waitForFinish = lua_toboolean(luaVM, 2);
         if(waitForFinish)
         {  /** \todo We need to handle the wait case, not just yield blindly.
             */
            return lua_yield(luaVM, 0);
         } 
      }

      DEBUG2("Setting region: ", regionName);
      tileEngine->setRegion(regionName);
   }

   return 0;
}

ScriptEngine::~ScriptEngine()
{  if(luaVM)
   {  DEBUG("Destroying Lua state machine...");
      lua_close(luaVM);
   }
}

ScriptEngine* ScriptEngine::getInstance() throw()
{  if(!instance)
   {  instance = new ScriptEngine();
   }

   return instance;
}

void ScriptEngine::runScript(std::string scriptName)
{  std::string scriptPath = scriptName + ".lua";//getScriptPath(scriptName);
   DEBUG2("Running script: ", scriptPath);
   luaL_dofile(luaVM, scriptPath.c_str());
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

std::string ScriptEngine::getScriptPath(std::string scriptName)
{  return scriptName + ".lua";
}

void ScriptEngine::destroy()
{   if(instance)
    {  delete instance;
       instance = NULL;
    }
}
