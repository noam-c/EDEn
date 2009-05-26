#ifndef __NPC_H_
#define __NPC_H_

#include <string>
#include <queue>
#include <map>

class Script;
class ScriptEngine;
struct lua_State;

/**
 * An NPC (non-player character) is an animate being on the map that can
 * respond to various user actions and follow groups of scripted instructions.
 *
 * NPCs each run a semi-coroutine, so that when scripts execute, they can yield
 * to the game code to complete instructions before resuming execution. This is
 * facilitated by an instruction queue and a coroutine provided for the NPC via
 * the scripting engine.
 *
 * @author Noam Chitayat
 */
class NPC
{  /**
    * Types of instructions that an NPC can perform. Each instruction
    * has a set of required parameters, which are noted in each instruction type.
    */
   enum InstructionType
   {  /** Instructs the NPC to move.
       * Parameters: X location to move to [int], Y location to move to [int]
       */
      MOVE,

      /** Instructs the NPC to say something.
       * Parameters: The text for the NPC to speak [string]
       */
      SPEAK
   };

   /**
    * A container for NPC instructions, as well as their parameters.
    */
   class Instruction
   {  InstructionType type;
      void* params;

      Instruction(InstructionType type, void* parameters)
                                  : type(type), params(parameters)
                                  {}
   };

   /** A list of all NPCs in the map, identified by their names */
   static std::map<std::string, NPC*> npcList;

   /** A queue of instructions for the NPC to follow */
   std::queue<Instruction*> instructions;

   /** The NPC's script */
   Script* npcScript;

   /** The scripting engine for the NPC to use */
   ScriptEngine* scriptEngine;

   public:
      /**
       * Constructor for the NPC.
       * Initializes the NPC's thread and loads the associated script.
       * 
       * @param name The name of the NPC (must also be the name of its script).
       */
      NPC(ScriptEngine* engine, lua_State* luaVM, std::string name);

      /**
       * A logic step for the NPC. Every frame, the NPC works on completing a
       * queued instruction (instructions may take several frames), and notifies
       * the scripting engine when the queue is empty. This function also alters
       * the sprite animation frame and NPC location.
       */
      void step();

      /**
       * The NPC is currently not doing anything, nor has it been asked to do
       * anything. It can run an "idle" function to do things like wander
       * aimlessly. The idle function, written in Lua, must not contain any
       * calls that would block users, such as speech or triggering a battle.
       * Otherwise, we would frustrate the hell out of a user.
       */
      void idle();

      /**
       * The NPC has been activated by the player, which usually indicates that
       * the user wishes to have their character speak to the NPC. The NPC needs
       * to stop what it's doing and run an "activate" function, in the Lua 
       * script to do things like speak, trigger battles, give items to the
       * player, etc.  
       */
      void activate();

      /**
       * This function draws the NPC in its current location with its current
       * sprite animation frame.
       */
      void draw();

      /**
       * This function enqueues a movement instruction.
       *
       * @param x The x coordinate (in tiles) for the NPC to move to
       * @param y The y coordinate (in tiles) for the NPC to move to
       */
      void orderMove(int x, int y);
};

#endif
