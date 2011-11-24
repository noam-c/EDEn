/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef NPC_H
#define NPC_H

#include <queue>
#include <string>

#include "MovementDirection.h"
#include "Point2D.h"

class Map;
class NPCScript;
class Scheduler;
class ScriptEngine;
class Sprite;
class Spritesheet;

/**
 * An NPC (non-player character) is an animate being on the map that can
 * respond to various user actions and follow groups of scripted instructions.
 *
 * NPCs each run a semi-coroutine, so that when scripts execute, they can yield
 * to the game code to complete instructions before resuming execution. This is
 * facilitated by an instruction queue and a coroutine provided for the NPC via
 * the scripting engine.
 *
 * How NPCs work:
 * NPCs contain NPCThreads, which are separate entities that managed the
 * thread/script state of an NPC's AI behaviour. An NPCThread contains (for now)
 * and runs an NPCScript based on whether or not the script is currently in the
 * middle of a run or if the NPC itself currently has instructions to work on.
 * If the script is not running and the NPC is idle, the NPCThread runs the
 * NPC's idle function (if it exists).
 * The NPC itself is updated by the TileEngine, and the NPCThread is given time
 * to run by the Scheduler as necessary, and updates the NPC's instruction queue
 * via the script instructions (which don't exist yet, but will).
 *
 * \todo Update this documentation when NPCThread and NPCScript are merged.
 *
 * @author Noam Chitayat
 */
class NPC
{
   /**
    * Types of instructions that an NPC can perform. Each instruction
    * has a set of required parameters, which are noted in each instruction type.
    */
   enum InstructionType
   {
      /** Instructs the NPC to move.
       * Parameters: X location to move to [int], Y location to move to [int]
       */
      MOVE,

      STAND,
   };

   /**
    * A container for NPC instructions, as well as their parameters.
    */
   struct Instruction
   {
      InstructionType type;
      void* params;

      Instruction(InstructionType type, void* parameters)
                                  : type(type), params(parameters)
                                  {}
   };

   /** A queue of instructions for the NPC to follow */
   std::queue<Instruction*> instructions;

   /** The NPC's associated sprite, which is drawn on screen. */
   Sprite* sprite;

   /** The NPC's thread of execution */
   NPCScript* npcThread;

   /** The NPC's name */
   std::string name;

   /** The map on which this NPC is located */
   const Map& map;

   Point2D pixelLoc;

   MovementDirection currDirection;

   /**
    * Run an NPC instruction for a frame.
    *
    * @param instruction The instruction for the NPC to run.
    * @param timePassed The amount of time that has passed since the last frame.
    */
   bool runInstruction(Instruction* instruction, long timePassed); 

   public:
      /**
       * Constructor for the NPC.
       * Initializes the NPC's thread and loads the associated script.
       * \todo Finish documenting.
       * 
       * @param name The name of the NPC (must also be the name of its script).
       */
      NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name, Spritesheet* sheet, const Map& map, const std::string& regionName, int x, int y);

      /**
       * @return The name of this NPC.
       */
      std::string getName();

      /**
       * A logic step for the NPC. Every frame, the NPC works on completing a
       * queued instruction (instructions may take several frames), and notifies
       * the scripting engine when the queue is empty. This function also alters
       * the sprite animation frame and NPC location.
       */
      void step();

      /**
       * Performs a logic step of this NPC. During the step, the NPC works on
       * enqueued Instructions if there are any.
       *
       * @param timePassed The amount of time that has passed since the last frame.
       */
      void step(long timePassed);

      /**
       * @return true iff the NPC is not chewing on any instructions
       *              (i.e. it is doing absolutely nothing)
       */
      bool isIdle();

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
       * @param x The x coordinate (in pixels) for the NPC to move to
       * @param y The y coordinate (in pixels) for the NPC to move to
       */
      void move(int x, int y);

      /**
       * This function changes the NPC's spritesheet.
       *
       * @param spritesheet A valid spritesheet.
       *
       */
      void setSpritesheet(Spritesheet* sheet);

      /**
       * This function changes the NPC's frame.
       *
       * @param frameName The name of the frame to use.
       *
       */
      void setFrame(const std::string& frameName);

      /**
       * This function changes the NPC's animation.
       *
       * @param animationName The name of the animation to use.
       *
       */
      void setAnimation(const std::string& animationName);

      /**
       * Destructor.
       */
      ~NPC();
};

#endif
