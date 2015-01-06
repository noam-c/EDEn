#include "TileEngineOverlay.h"

TileEngineOverlay::TileEngineOverlay(messaging::MessagePipe& messagePipe, PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rocket::Core::Context& rocketContext) :
   m_consoleWindow(messagePipe, rocketContext),
   m_shortcutBar(playerData, metadata, stateType, rocketContext)
{
}

void TileEngineOverlay::refresh()
{
   m_shortcutBar.refresh();
}

void TileEngineOverlay::toggleDebugConsole()
{
   bool consoleWindowVisible = m_consoleWindow.isVisible();
   if(consoleWindowVisible)
   {
      m_consoleWindow.hide();
   }
   else
   {
      m_consoleWindow.show();
   }
}

int TileEngineOverlay::getShortcutBarHeight()
{
   return m_shortcutBar.getHeight();
}

bool TileEngineOverlay::isDebugConsoleVisible()
{
   return m_consoleWindow.isVisible();
}
