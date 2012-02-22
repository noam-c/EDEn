#include "LuaNPC.h"
#include "NPC.h"
#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int NPCL_Move(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 3:
      {
         NPC* npc = luaW_check<NPC>(luaVM, 1);
         if (npc)
         {
            int x = lua_tointeger(luaVM, 2);
            int y = lua_tointeger(luaVM, 3);
            npc->move(x, y);
         }
         break;
      }
   }
   return 0;
}

static luaL_reg npcMetatable[] =
{
   { "move", NPCL_Move },
   { NULL, NULL }
};

void luaopen_NPC(lua_State* luaVM)
{
   luaW_register<NPC>(luaVM, "NPC", NULL, npcMetatable, NULL, NULL);
}
