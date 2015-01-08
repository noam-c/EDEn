/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "DialogueBox.h"

#include <Rocket/Core.h>

#include "DialogueController.h"

DialogueBox::DialogueBox(Rocket::Core::Element* dialogueBox, DialogueController& controller) :
   m_dialogueBox(dialogueBox),
   m_controller(controller)
{
   if(m_dialogueBox != nullptr)
   {
      m_dialogueTextArea = m_dialogueBox->GetElementById("textArea");
   }
}

void DialogueBox::refresh()
{
   bool shouldBeShown = m_controller.hasDialogue();
   m_dialogueTextArea->SetClass("collapsed", !shouldBeShown);

   if(shouldBeShown)
   {
      bool isSpeech = m_controller.getEntryType() == DialogueEntryType::SAY;
      m_dialogueBox->SetClass("speech", isSpeech);
      m_dialogueBox->SetClass("narration", !isSpeech);
   }
   
   m_dialogueBox->SetClass("dialogueComplete", m_controller.isCurrentLineComplete());
   
   // Display the necessary piece of text in the text box
   std::string dialogue = m_controller.getTextToShow();
   m_dialogueTextArea->SetInnerRML(dialogue.c_str());
}
