#include "ScriptEngine.h"
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

#define REGISTER(luaName, function) DEBUG2("Registering function: ", luaName); \
                                    lua_register(luaVM, luaName, function)

static int luaNarrate(lua_State* luaVM)
{  return ScriptEngine::getInstance()->narrate(luaVM);
}

static int luaSay(lua_State* luaVM)
{  return ScriptEngine::getInstance()->say(luaVM);
}

static int luaSetRegion(lua_State* luaVM)
{  return ScriptEngine::getInstance()->setRegion(luaVM);
}

void ScriptEngine::registerFunctions()
{  REGISTER("narrate", luaNarrate);
   REGISTER("say", luaSay);
   REGISTER("setRegion", luaSetRegion);
//   REGISTER("registerNPC", luaRegisterNPC);
//   REGISTER("playMusic", luaPlayMusic);
//   REGISTER("setMap", luaSetMap);
}
