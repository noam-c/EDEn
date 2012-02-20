#include "LuaTileEngine.h"
#include "TileEngine.h"
#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int TileEngineL_TilesToPixels(lua_State* luaVM)
{
   TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
   if (tileEngine)
   {
      int tileNum = luaL_checknumber(luaVM, 2);
      lua_pushnumber(luaVM, TileEngine::TILE_SIZE * tileNum);
      return 1;
   }

   return 0;
}

static luaL_reg tileEngineMetatable[] =
{
   { "tilesToPixels", TileEngineL_TilesToPixels },
   { NULL, NULL }
};

void luaopen_TileEngine(lua_State* luaVM)
{
   luaW_register<TileEngine>(luaVM, "TileEngine", NULL, tileEngineMetatable, NULL, NULL);
}
