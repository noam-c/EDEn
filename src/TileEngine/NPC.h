/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef NPC_H
#define NPC_H

#include "GridActor.h"
#include <memory>

class NPCScript;
class Scheduler;
class ScriptEngine;

/**
 * An NPC (non-player character) is an animate being on the map that can
 * respond to various user actions and follow groups of scripted instructions.
 *
 * NPCs each run a semi-coroutine, so that when scripts execute, they can yield
 * to the game code to complete instructions before resuming execution. This is
 * facilitated by a task queue and a coroutine provided for the NPC via
 * the scripting engine.
 *
 * How NPCs work:
 * NPCs contain NPCScripts, which are separate coroutines that manage the
 * script state of an NPC's AI behaviour. An NPCScript runs based on whether
 * or not the script is currently in the middle of a run or if the NPC itself
 * currently has instructions to work on.
 * If the script is not running and the NPC is idle, the NPCCoroutine runs the
 * NPC's idle function (if it exists).
 * The NPC itself is updated by Lua bindings, and the NPCCoroutine is given time
 * to run by the Scheduler as necessary, and updates the NPC's instruction queue
 * via the script instructions.
 *
 * @author Noam Chitayat
 */
class NPC final : public GridActor
{
   /** The NPC's coroutine of execution */
   std::shared_ptr<NPCScript> m_coroutine;

   public:
      /**
       * Constructor for the NPC.
       * Initializes the NPC's coroutine and loads the associated script.
       * Loads a sprite for the NPC based on a given Spritesheet.
       *
       * @param engine The scripting engine that provides the NPC's coroutine.
       * @param scheduler The scheduler that owns the NPC's coroutine
       * @param name The name of the NPC (must also be the name of its script).
       * @param direction The direction that the NPC will face at first.
       * @param sheetName The name of the spritesheet to use for rendering the NPC.
       * @param messagePipe The message pipe used to send event messages.
       * @param entityGrid The map that this NPC will be interacting in.
       * @param regionName The name of the region that this NPC is interacting in.
       * @param location The location (in pixels) where the NPC will start off.
       * @param size The size (in pixels) of the NPC.
       */
      NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name,
          const geometry::Direction direction, const std::string& sheetName,
          messaging::MessagePipe& messagePipe, EntityGrid& entityGrid,
          const std::string& regionName, const geometry::Point2D& location,
          const geometry::Size& size);

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
       * Destructor.
       */
      ~NPC() override;
};

#endif
