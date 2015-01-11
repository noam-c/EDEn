#ifndef TILE_ENGINE_OVERLAY_H
#define TILE_ENGINE_OVERLAY_H

#include <memory>

#include "DebugConsoleWindow.h"
#include "DialogueBox.h"
#include "GameStateType.h"
#include "ShortcutBar.h"

class PlayerData;
class Metadata;

namespace Rocket
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
 * over top of the TileEngine.
 *
 * @author Noam Chitayat
 */
class TileEngineOverlay
{
   /** The RML document that represents the overlay GUI. */
   Rocket::Core::ElementDocument* m_overlayDocument;

   /** The debug console window to be used for diagnostics. */
   DebugConsoleWindow m_consoleWindow;

   /** The active dialogue box instance, if one was built for this overlay. */
   std::shared_ptr<DialogueBox> m_dialogueBox;

   /** The window containing the player's shortcuts. */
   ShortcutBar m_shortcutBar;

   public:
      /**
       * Constructor.
       * Builds the overlay GUI elements.
       */
      TileEngineOverlay(messaging::MessagePipe& messagePipe, PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rocket::Core::Context& rocketContext, DialogueController& dialogueController);

      /**
       * Destructor. Cleans up RML documents loaded by the overlay.
       */
      ~TileEngineOverlay();

      /**
       * @return The view managing the TileEngine's dialogue box.
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
       * @return the height of the shortcut bar.
       */
      int getShortcutBarHeight();

      /**
       * @return true iff the debug console is currently visible.
       */
      bool isDebugConsoleVisible();
};

#endif
