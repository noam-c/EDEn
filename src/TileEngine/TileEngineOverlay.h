#ifndef TILE_ENGINE_OVERLAY_H
#define TILE_ENGINE_OVERLAY_H

#include "DebugConsoleWindow.h"
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

class TileEngineOverlay
{
   /** The debug console window to be used for diagnostics. */
   DebugConsoleWindow m_consoleWindow;
   
   /** The window containing the player's shortcuts. */
   ShortcutBar m_shortcutBar;
   
   public:
      /**
       * Constructor.
       *
       * @param rocketContext The Rocket context to which this console window should attach.
       */
      TileEngineOverlay(PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rocket::Core::Context& rocketContext);
   
      void refresh();

      /**
       * Toggles the debug console on or off.
       */
      void toggleDebugConsole();
   
      int getShortcutBarHeight();
   
      bool isDebugConsoleVisible();
};

#endif
