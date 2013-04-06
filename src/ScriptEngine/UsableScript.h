/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef USABLE_SCRIPT_H
#define USABLE_SCRIPT_H

#include <vector>

#include "Script.h"

class Usable;
class Character;

/**
 * An <code>UsableScript</code> holds functions that determine <code>Usable</code> behaviour.
 *
 * @author Noam Chitayat
 */
class UsableScript : public Script
{
   /**
    * The functions that can be called on an Usable.
    * When new functions are added here, the name of the function must also be
    * added to FUNCTION_NAMES. The functions are optional and can be replaced
    * by an empty function if they do not exist.
    */
   enum UsableFunction
   {
      /** The function for using an usable in the menu. */
      MENU_USE,
      /** The function for using an usable in the field. */
      FIELD_USE,
      /** The function for using an usable in the battle. */
      BATTLE_USE,
      /** The number of UsableFunction values. */
      NUM_FUNCTIONS
   };

   /**
    * The names of the functions denoted in the Functions list.
    */
   static const char* FUNCTION_NAMES[];

   /** The usable that runs this script. */
   const Usable& usable;

   /**
    * A mapping to determine whether or not each function exists and
    * has been properly loaded for the Usable.
    * Indexing is done using the UsableFunction enum.
    * (e.g. functionExists[MENU_USE] returns true iff the Usable script had a menu usage
    * function declared)
    */
   std::vector<bool> functionExists;

   /**
    * Call a function on this usable's script.
    *
    * @param function The function to call
    * @param usingCharacter The character using the usable for this call.
    *
    * @return true iff the script runs to completion, false if the coroutine
    *         yielded, or there was an error in execution.
    */
   bool callFunction(UsableFunction function, Character* usingCharacter = NULL);

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
       * @param usable The usable that uses this script
       */
      UsableScript(lua_State* luaVM, const std::string& scriptPath, const Usable& usable);

      /**
       * Destructor.
       */
      virtual ~UsableScript();

      bool onMenuUse(Character* usingCharacter = NULL);

      bool onFieldUse(Character* usingCharacter = NULL);

      bool onBattleUse(Character* usingCharacter = NULL);
};

#endif
