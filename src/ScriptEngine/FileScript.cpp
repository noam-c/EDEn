#include "FileScript.h"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
   #include <lua.h>
   #include <lualib.h>
   #include <lauxlib.h>
}

FileScript::FileScript(lua_State* luaVM, std::string scriptPath)
{  luaStack = lua_newthread(luaVM);
   luaL_loadfile(luaStack, scriptPath.c_str());
}

FileScript::~FileScript()
{
}
