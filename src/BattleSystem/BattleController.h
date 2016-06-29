/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef BATTLE_CONTROLLER_H
#define BATTLE_CONTROLLER_H

#include "GameState.h"
#include "BattleOverlay.h"
#include "DialogueController.h"
#include "MessagePipe.h"
#include "Task.h"

#include <map>
#include <memory>
#include <string>

class PlayerData;

/**
 * GameState that manages combat in the game.
 *
 * @author Noam Chitayat
 */
class BattleController final :
   public GameState,
   public std::enable_shared_from_this<BattleController>
{
   std::shared_ptr<PlayerData> m_playerData;
   std::shared_ptr<Task> m_battleTask;
   
   messaging::MessagePipe m_messagePipe;

   /** True iff the BattleController has already been initialized. */
   bool m_initialized;

   /** The coroutine scheduler used by the combat system. */
   Scheduler m_scheduler;

   /** Controller for dialogue and narrations. */
   DialogueController m_dialogue;

   /** The GUI overlaid on top of the battle. */
   BattleOverlay m_overlay;
   
   /**
    * Handles input events specific to the tile engine.
    *
    * @param finishState Returned as true if the input event quit out of the tile engine.
    */
   void handleInputEvents(bool& finishState);

   protected:
      /**
       * Activation of state.
       * Rebinds the BattleController's message pipe to the player data.
       */
      void activate() override;

      /**
       * Deactivation of state.
       * Unbinds the BattleController's message pipe from the player data.
       */
      void deactivate() override;

      /**
       * Logic step.
       * Sends time passed to all controllers so that they can update accordingly.
       * Takes user input if there is any.
       */
      bool step(long timePassed) override;

      /**
       * Draw map tiles if a map is loaded in, and then coordinate the drawing
       * of all the controllers and widgets.
       */
      void draw() override;

   public:
      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack.
       * @param playerData The currently loaded player data.
       */
      BattleController(GameContext& gameContext, std::shared_ptr<PlayerData> playerData, std::shared_ptr<Task> battleTask = nullptr);

      /**
       * Destructor.
       */
      ~BattleController() override;

      /**
       * @return The tile engine's coroutine scheduler.
       */
      Scheduler* getScheduler() override;

      /**
       * Send a line of dialogue to the DialogueController as a narration.
       *
       * @param narration The line of dialogue to appear as a narration.
       * @param task The ticket of this narration instruction
       */
      void dialogueNarrate(const std::string& narration, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices);

      /**
       * Send a line of dialogue to the DialogueController as speech.
       *
       * @param speech The line of dialogue to appear as character speech.
       * @param task The ticket of this speech instruction
       */
      void dialogueSay(const std::string& speech, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices);
};

#endif
