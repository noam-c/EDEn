#include "LuaInventory.h"
#include "Inventory.h"

#include "LuaWrapper.hpp"

// Include the Lua libraries. Since they are written in clean C, the functions
// need to be included in this fashion to work with the C++ code.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

static int InventoryL_GetItemCount(lua_State* luaVM)
{
   int itemCount = 0;
   const int nargs = lua_gettop(luaVM);
   switch(nargs)
   {
      case 2:
      {
         const Inventory* inv = luaW_to<Inventory>(luaVM, 1);
         if(inv != NULL)
         {
            const int itemId = lua_tonumber(luaVM, 2);
            itemCount = inv->getItemQuantity(itemId);
         }
         break;
      }
   }
   
   lua_pushnumber(luaVM, itemCount);
   return 1;
}

static int InventoryL_AddItem(lua_State* luaVM)
{
   bool success = false;
   const int nargs = lua_gettop(luaVM);
   Inventory* inv;
   
   if(nargs > 1)
   {
      inv = luaW_to<Inventory>(luaVM, 1);
      switch(nargs)
      {
         case 2:
         {
            success = inv->addItem(lua_tonumber(luaVM, 2));
            break;
         }
         case 3:
         {
            success = inv->addItem(lua_tonumber(luaVM, 2), lua_tonumber(luaVM, 3));
            break;
         }
      }      
   }
   
   lua_pushboolean(luaVM, success);
   return 1;
}

static int InventoryL_RemoveItem(lua_State* luaVM)
{
   bool success = false;
   const int nargs = lua_gettop(luaVM);
   Inventory* inv;
   
   if(nargs > 1)
   {
      inv = luaW_to<Inventory>(luaVM, 1);
      switch(nargs)
      {
         case 2:
         {
            success = inv->removeItem(lua_tonumber(luaVM, 2));
            break;
         }
         case 3:
         {
            success = inv->removeItem(lua_tonumber(luaVM, 2), lua_tonumber(luaVM, 3));
            break;
         }
      }      
   }
   
   lua_pushboolean(luaVM, success);
   return 1;
}

static luaL_reg inventoryMetatable[] =
{
   { "getItemCount", InventoryL_GetItemCount },
   { "addItem", InventoryL_AddItem },
   { "removeItem", InventoryL_RemoveItem },
   { NULL, NULL }
};

void luaopen_Inventory(lua_State* luaVM)
{
   luaW_register<Inventory>(luaVM, "Inventory", NULL, inventoryMetatable, NULL, NULL);
}
