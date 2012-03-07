#include "LuaPlayerCharacter.h"
#include "PlayerCharacter.h"
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

static int PlayerCharacterL_Show(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 3:
      {
         PlayerCharacter* playerCharacter = luaW_check<PlayerCharacter>(luaVM, 1);
         if (playerCharacter)
         {
            int x = lua_tointeger(luaVM, 2);
            int y = lua_tointeger(luaVM, 3);
            playerCharacter->addToMap(shapes::Point2D(x, y));
         }
         break;
      }
   }
   return 0;
}

static int PlayerCharacterL_Remove(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 1:
      {
         PlayerCharacter* playerCharacter = luaW_check<PlayerCharacter>(luaVM, 1);
         if (playerCharacter)
         {
            playerCharacter->removeFromMap();
         }
         break;
      }
   }
   return 0;
}

static int PlayerCharacterL_SetLocation(lua_State* luaVM)
{
   int nargs = lua_gettop(luaVM);
   
   switch(nargs)
   {
      case 3:
      {
         PlayerCharacter* playerCharacter = luaW_check<PlayerCharacter>(luaVM, 1);
         if (playerCharacter)
         {
            int x = lua_tointeger(luaVM, 2);
            int y = lua_tointeger(luaVM, 3);
            playerCharacter->setLocation(shapes::Point2D(x, y));
         }
         break;
      }
   }
   return 0;
}

static luaL_reg playerCharacterMetatable[] =
{
   { "show", PlayerCharacterL_Show },
   { "remove", PlayerCharacterL_Remove },
   { "setLocation", PlayerCharacterL_SetLocation },
   { NULL, NULL }
};

void luaopen_PlayerCharacter(lua_State* luaVM)
{
   luaW_register<PlayerCharacter>(luaVM, "PlayerCharacter", NULL, playerCharacterMetatable, NULL, NULL);
   luaW_extend<PlayerCharacter, Actor>(luaVM);
}
