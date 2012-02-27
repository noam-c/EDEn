#include "LuaTileEngine.h"
#include "TileEngine.h"
#include "NPC.h"
#include "Point2D.h"
#include "LuaWrapper.hpp"

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

static int TileEngineL_AddNPC(lua_State* luaVM)
{
   NPC* npc = NULL;
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 5:
      {
         TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
         if (tileEngine)
         {
            std::string npcName(lua_tostring(luaVM, 2));
            std::string spritesheetName(lua_tostring(luaVM, 3));
            int x = lua_tointeger(luaVM, 4);
            int y = lua_tointeger(luaVM, 5);

            DEBUG("Adding NPC %s with spritesheet %s", npcName.c_str(), spritesheetName.c_str());
            DEBUG("NPC Location will be (%d, %d)", x, y);
            npc = tileEngine->addNPC(npcName, spritesheetName, Point2D(x, y));
         }
      }
   }
   
   luaW_push<Actor>(luaVM, npc);
   return 1;
}

static int TileEngineL_GetNPC(lua_State* luaVM)
{
   NPC* npc = NULL;
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 5:
      {
         TileEngine* tileEngine = luaW_check<TileEngine>(luaVM, 1);
         if (tileEngine)
         {
            std::string npcName(lua_tostring(luaVM, 2));
            npc = tileEngine->getNPC(npcName);
         }
      }
   }

   luaW_push<NPC>(luaVM, npc);
   return 1;
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
   { "addNPC", TileEngineL_AddNPC },
   { "getNPC", TileEngineL_GetNPC },
   { "tilesToPixels", TileEngineL_TilesToPixels },
   { NULL, NULL }
};

void luaopen_TileEngine(lua_State* luaVM)
{
   luaW_register<TileEngine>(luaVM, "TileEngine", NULL, tileEngineMetatable, NULL, NULL);
}
