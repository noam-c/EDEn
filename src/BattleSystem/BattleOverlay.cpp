#include "BattleOverlay.h"

BattleOverlay::BattleOverlay(messaging::MessagePipe& messagePipe, PlayerData& playerData, Metadata& metadata, GameStateType stateType, Rocket::Core::Context& rocketContext, DialogueController& dialogueController) :
   m_consoleWindow(messagePipe, rocketContext)
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

BattleOverlay::~BattleOverlay()
{
   if(m_overlayDocument != nullptr)
   {
      m_overlayDocument->RemoveReference();
      m_overlayDocument->Close();
   }
}

std::shared_ptr<DialogueBox>& BattleOverlay::getDialogueBox()
{
   return m_dialogueBox;
}

void BattleOverlay::refresh()
{
   m_dialogueBox->refresh();
}

void BattleOverlay::toggleDebugConsole()
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

bool BattleOverlay::isDebugConsoleVisible()
{
   return m_consoleWindow.isVisible();
}
