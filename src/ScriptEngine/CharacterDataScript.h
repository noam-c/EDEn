/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef CHARACTER_DATA_SCRIPT_H
#define CHARACTER_DATA_SCRIPT_H

#include <map>
#include <vector>

#include "Script.h"

class Character;

/**
 * An <code>CharacterScript</code> holds functions that determine general <code>Character</code> behaviour,
 * such as setting up stats and custom events.
 *
 * @author Noam Chitayat
 */
class CharacterDataScript : public Script
{
   /**
    * The functions that can be called on an Usable.
    * When new functions are added here, the name of the function must also be
    * added to FUNCTION_NAMES. The functions are optional and can be replaced
    * by an empty function if they do not exist.
    */
   enum class CharacterDataFunction
   {
      /** The function for initializing a created or loaded character. */
      INITIALIZE,
      /** The number of CharacterFunction values. */
      NUM_FUNCTIONS
   };

   /**
    * The names of the functions denoted in the Functions list.
    */
   static const char* FUNCTION_NAMES[];

   /**
    * A mapping to determine whether or not each function exists and
    * has been properly loaded.
    * Indexing is done using the CharacterFunction enum.
    * (e.g. functionExists[INITIALIZE] returns true iff the Character script has an initialization
    * function declared)
    */
   std::vector<bool> m_functionExists;

   /**
    * Call a function on this Character script.
    *
    * @param function The function to call
    * @param character The character being acted upon for this call.
    *
    * @return true iff the script runs to completion, false if the coroutine
    *         yielded, or there was an error in execution.
    */
   bool callFunction(CharacterDataFunction function, Character* character);

   public:
      /**
       * Constructor.
       * Creates a new Lua coroutine by forking the main VM, and then
       * loads the specified script file's functions into the Lua environment.
       * The functions are then moved from the environment globals table into
       * a unique table specifically reserved for this Character data.
       *
       * @param luaVM The main Lua stack to fork a coroutine from.
       * @param scriptPath The path to a script that should be run on this coroutine.
       */
      CharacterDataScript(lua_State* luaVM, const std::string& scriptPath);

      /**
       * Destructor.
       */
      ~CharacterDataScript() override;

      bool initialize(Character* character);
};

#endif
