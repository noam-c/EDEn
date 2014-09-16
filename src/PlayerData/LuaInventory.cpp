/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "LuaInventory.h"
#include "Inventory.h"

#include "ScriptUtilities.h"

static int InventoryL_GetItemCount(lua_State* luaVM)
{
   const Inventory* inventory = luaW_check<Inventory>(luaVM, 1);
   if(inventory == nullptr)
   {
      return lua_error(luaVM);
   }

   int itemId;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "id", itemId))
   {
      return lua_error(luaVM);
   }

   const int itemCount = inventory->getItemQuantity(itemId);
   lua_pushnumber(luaVM, itemCount);
   return 1;
}

static int InventoryL_AddItem(lua_State* luaVM)
{
   Inventory* inventory = luaW_check<Inventory>(luaVM, 1);
   if(inventory == nullptr)
   {
      return lua_error(luaVM);
   }

   int itemId;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "id", itemId))
   {
      return lua_error(luaVM);
   }

   int itemQuantity;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "quantity", itemQuantity))
   {
      return lua_error(luaVM);
   }

   const bool success = inventory->addItem(itemId, itemQuantity);
   lua_pushboolean(luaVM, success);
   return 1;
}

static int InventoryL_RemoveItem(lua_State* luaVM)
{
   Inventory* inventory = luaW_check<Inventory>(luaVM, 1);
   if(inventory == nullptr)
   {
      return lua_error(luaVM);
   }

   int itemId;
   if(!ScriptUtilities::getParameter(luaVM, 2, 1, "id", itemId))
   {
      return lua_error(luaVM);
   }

   int itemQuantity;
   if(!ScriptUtilities::getParameter(luaVM, 2, 2, "quantity", itemQuantity))
   {
      return lua_error(luaVM);
   }

   const bool success = inventory->removeItem(itemId, itemQuantity);
   lua_pushboolean(luaVM, success);
   return 1;
}

static luaL_Reg inventoryMetatable[] =
{
   { "getItemCount", InventoryL_GetItemCount },
   { "addItem", InventoryL_AddItem },
   { "removeItem", InventoryL_RemoveItem },
   { nullptr, nullptr }
};

void luaopen_Inventory(lua_State* luaVM)
{
   luaW_register<Inventory>(luaVM, "Inventory", nullptr, inventoryMetatable, nullptr, nullptr);
}
