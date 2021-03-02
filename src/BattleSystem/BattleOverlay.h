/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef BATTLE_OVERLAY_H
#define BATTLE_OVERLAY_H

#include <memory>

#include "DebugConsoleWindow.h"
#include "DialogueBox.h"
#include "ShortcutBar.h"

class PlayerData;
class Metadata;

namespace Rml
{
   namespace Core
   {
      class Context;
   };
};

namespace messaging
{
   class MessagePipe;
};

/**
 * Manages the collection of GUI elements that will be displayed
 * over top of the battle.
 *
 * @author Noam Chitayat
 */
class BattleOverlay final
{
   /** The RML document that represents the overlay GUI. */
   Rml::Core::ElementDocument* m_overlayDocument;

   /** The debug console window to be used for diagnostics. */
   DebugConsoleWindow m_consoleWindow;

   /** The active dialogue box instance, if one was built for this overlay. */
   std::shared_ptr<DialogueBox> m_dialogueBox;

   public:
      /**
       * Constructor.
       * Builds the overlay GUI elements.
       */
      BattleOverlay(messaging::MessagePipe& messagePipe, PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rml::Core::Context& rocketContext, DialogueController& dialogueController);

      /**
       * Destructor.
       */
      ~BattleOverlay();

      /**
       * @return The view managing the Battle's dialogue box.
       */
      std::shared_ptr<DialogueBox>& getDialogueBox();

      /**
       * Refreshes the overlay GUI elements.
       */
      void refresh();

      /**
       * Toggles the debug console on or off.
       */
      void toggleDebugConsole();

      /**
       * @return true iff the debug console is currently visible.
       */
      bool isDebugConsoleVisible();
};

#endif
