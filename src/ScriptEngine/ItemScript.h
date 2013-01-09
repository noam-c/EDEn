/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_SCRIPT_H
#define ITEM_SCRIPT_H

#include <vector>

#include "Script.h"

class Item;

/**
 * An <code>ItemScript</code> holds functions that determine <code>Item</code> behaviour.
 *
 * @author Noam Chitayat
 */
class ItemScript : public Script
{
   /**
    * The functions that can be called on an Item.
    * When new functions are added here, the name of the function must also be
    * added to FUNCTION_NAMES. The functions are optional and can be replaced
    * by an empty function if they do not exist.
    */
   enum ItemFunction
   {
      /** The function for using an item in the menu. */
      MENU_USE,
      /** The function for using an item in the field. */
      FIELD_USE,
      /** The function for using an item in the battle. */
      BATTLE_USE,
      /** The number of ItemFunction values. */
      NUM_FUNCTIONS
   };

   /**
    * The names of the functions denoted in the Functions list.
    */
   static const char* FUNCTION_NAMES[];

   /** The item that runs this script. */
   const Item& item;

   /**
    * A mapping to determine whether or not each function exists and
    * has been properly loaded for the Item.
    * Indexing is done using the ItemFunction enum.
    * (e.g. functionExists[MENU_USE] returns true iff the Item script had a menu usage
    * function declared)
    */
   std::vector<bool> functionExists;

   /**
    * Call a function on this item's script.
    *
    * @param function the function to call
    *
    * @return true iff the script runs to completion, false if the coroutine
    *         yielded, or there was an error in execution.
    */
   bool callFunction(ItemFunction function);

   public:
      /**
       * Constructor.
       * Creates a new Lua coroutine by forking the main VM, and then
       * loads the specified script file's NPC functions into the Lua environment.
       * The functions are then moved from the environment globals table into
       * a unique table specifically reserved for this NPC.
       *
       * @param luaVM The main Lua stack to fork a coroutine from.
       * @param scriptPath The path to a script that should be run on this coroutine.
       * @param item The item that uses this script
       */
      ItemScript(lua_State* luaVM, const std::string& scriptPath, const Item& item);

      /**
       * Destructor.
       */
      ~ItemScript();

      bool onMenuUse();

      bool onFieldUse();

      bool onBattleUse();
};

#endif
