#ifndef __NPC_SCRIPT_H_
#define __NPC_SCRIPT_H_

#include "Script.h"

/**
 * An NPCScript is a type of Script that holds functions that determine
 * NPC behaviour.
 *
 * @author Noam Chitayat
 */
class NPCScript : public Script
{  /**
    * The functions that can be called on an NPC.
    * When new functions are added here, the name of the function must also be
    * added to FUNCTION_NAMES. The functions are optional and can be replaced
    * by an empty function if they do not exist in a given NPC.
    */
   enum Functions
   {  IDLE,
      NUM_FUNCTIONS
   };

   /**
    * The names of the functions denoted in the Functions list.
    */
   static const char* FUNCTION_NAMES[];

   public:
      /**
       * Constructor.
       * Creates a new Lua thread by forking the main VM, and then
       * loads the specified script file's NPC functions into the Lua environment.
       * The functions are then moved from the environment globals table into
       * a unique table specifically reserved for this NPC.
       *
       * @param luaVM The main Lua stack to fork a thread from.
       * @param scriptPath The path to a script that should be run on this thread.
       */
      NPCScript(lua_State* luaVM, std::string scriptPath);

      /**
       * Call a function on this NPC's script.
       *
       * @param functionName the name of the function to call
       *
       * @return true iff the script runs to completion, false if the coroutine
       *         yielded, or there was an error in execution.
       */
      bool callFunction(std::string functionName);

      /**
       * Destructor.
       */
      ~NPCScript();
};

#endif
