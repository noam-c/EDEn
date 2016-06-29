/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "TileEngineOverlay.h"

TileEngineOverlay::TileEngineOverlay(messaging::MessagePipe& messagePipe, PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rocket::Core::Context& rocketContext, DialogueController& dialogueController) :
   m_consoleWindow(messagePipe, rocketContext),
   m_shortcutBar(playerData, metadata, stateType, rocketContext)
{
   m_overlayDocument = rocketContext.LoadDocument("data/gui/tileEngineOverlay.rml");

   if(m_overlayDocument != nullptr)
   {
      Rocket::Core::Element* dialogueBoxElement = m_overlayDocument->GetElementById("dialogueBox");

      if(dialogueBoxElement != nullptr)
      {
         m_dialogueBox = std::make_shared<DialogueBox>(dialogueBoxElement, dialogueController);
      }

      m_overlayDocument->Show();
   }
}

TileEngineOverlay::~TileEngineOverlay()
{
   if(m_overlayDocument != nullptr)
   {
      m_overlayDocument->RemoveReference();
      m_overlayDocument->Close();
   }
}

std::shared_ptr<DialogueBox>& TileEngineOverlay::getDialogueBox()
{
   return m_dialogueBox;
}

void TileEngineOverlay::refresh()
{
   m_dialogueBox->refresh();
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
