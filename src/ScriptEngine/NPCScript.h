/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef NPC_SCRIPT_H
#define NPC_SCRIPT_H

#include <vector>

#include "Script.h"

class NPC;

/**
 * An <code>NPCScript</code> holds functions that determine <code>NPC</code> behaviour,
 * while also controlling when an <code>NPC</code> executes these functions.
 *
 * When resumed, instead of strictly running the script, the <code>NPCScript</code> only
 * runs the script if it is in the middle of execution. Otherwise, it checks if
 * the <code>NPC</code> is busy and runs the <code>NPC</code>'s idle script function
 * if it is not doing anything.
 *
 * The <code>NPCScript</code> and <code>NPC</code> actor need to be separate entities,
 * because otherwise the <code>Scheduler</code> could block the <code>NPC</code> in its
 * entirety if the script executes a blocking instruction.
 *
 * For example, if the <code>NPCScript</code> requests that the <code>NPC</code>
 * move to a location before continuing execution, the <code>NPC</code> actor coroutine
 * needs to run in order to complete movement while the <code>NPCScript</code>
 * is blocked.
 *
 * @author Noam Chitayat
 */
class NPCScript final : public Script
{
   /**
    * The functions that can be called on an NPC.
    * When new functions are added here, the name of the function must also be
    * added to FUNCTION_NAMES. The functions are optional and can be replaced
    * by an empty function if they do not exist in a given NPC.
    */
   enum class NPCFunction
   {
      /** The idling function for the NPC (runs if the NPC is not running any other instructions). */
      IDLE,
      /** The activation function for the NPC (runs if the NPC is 'clicked' by the player) */
      ACTIVATE,
      /** The number of NPCFunction values. */
      NUM_FUNCTIONS
   };

   /**
    * The names of the functions denoted in the Functions list.
    */
   static const char* FUNCTION_NAMES[];

   /**
    * A mapping to determine whether or not each function exists and
    * has been properly loaded for the NPC.
    * Indexing is done using the NPCFunction enum.
    * (e.g. functionExists[IDLE] returns true iff the NPC script had an idle
    * function declared)
    */
   std::vector<bool> m_functionExists;

   /** The NPC controlled by this script's execution. */
   NPC* m_npc;

   /** True iff the NPC script received a signal to call the NPC's activate function. */
   bool m_activated;

   /** True iff the NPC script is finished and should be unscheduled. */
   bool m_finished;

   static constexpr std::underlying_type<NPCFunction>::type getNumFunctions();

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
       * @param npc The NPC controlled by this coroutine.
       */
      NPCScript(lua_State* luaVM, const std::string& scriptPath, NPC* npc);

      /**
       * Destructor.
       */
      ~NPCScript() override;

      /**
       * Call a function on this NPC's script.
       *
       * @param function the function to call
       *
       * @return true iff the script runs to completion, false if the coroutine
       *         yielded, or there was an error in execution.
       */
      bool callFunction(NPCFunction function);

      /**
       * Either resume the NPC's script if it is running, or run the script's
       * idle function if the NPC isn't doing anything.
       *
       * @param timePassed the amount of time that has passed since the last frame.
       *
       * @return true iff the NPC is finished and the coroutine should end.
       */
      bool resume(long timePassed) override;

      /**
       * Activates this NPC as a result of player action.
       * Call the activate function in the script and stall the current idle
       * instructions.
       */
      void activate();

      /**
       * Signal that the NPC is finished, and its coroutine should no longer
       * execute.
       */
      void finish();
};

#endif
