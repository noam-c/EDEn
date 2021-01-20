/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "DialogueBox.h"

#include <RmlUi/Core.h>
#include <RmlUi/Controls.h>

#include "DialogueController.h"

DialogueBox::DialogueBox(Rml::Core::Element* dialogueBox, DialogueController& controller) :
   m_dialogueBox(dialogueBox),
   m_controller(controller),
   m_choicesDataSource(controller)
{
   if(m_dialogueBox != nullptr)
   {
      m_dialogueTextArea = m_dialogueBox->GetElementById("textArea");
      m_dialogueChoiceList = m_dialogueBox->GetElementById("choiceList");
      m_bindings.bindAction(m_dialogueChoiceList, "click", [this](Rml::Core::Event& event) { onChoiceListClicked(event); });
   }
}

void DialogueBox::onDialogueAdvanced()
{
   m_dialogueBox->SetClass("collapsed", !m_controller.hasDialogue());

   // Display the necessary piece of text in the text box
   std::string dialogue = m_controller.getTextToShow();
   m_dialogueTextArea->SetInnerRML(dialogue.c_str());

   m_dialogueBox->SetClass("dialogueComplete", m_controller.isCurrentLineComplete());
}

void DialogueBox::onDialogueChanged()
{
   if(m_controller.hasDialogue())
   {
      bool isSpeech = m_controller.getEntryType() == DialogueEntryType::SAY;
      m_dialogueBox->SetClass("speech", isSpeech);
      m_dialogueBox->SetClass("narration", !isSpeech);

      m_dialogueBox->SetClass("noChoices", !m_controller.hasChoices());
   }

   onDialogueAdvanced();
   m_choicesDataSource.refresh();
}

void DialogueBox::refresh()
{
   onDialogueChanged();
}

void DialogueBox::onChoiceListClicked(Rml::Core::Event& event)
{
   auto target = event.GetTargetElement();

   // Move up the DOM to the datagridrow item holding this element
   while(target->GetParentNode() != nullptr && target->GetTagName() != "datagridrow")
   {
      target = target->GetParentNode();
   }

   if(target != nullptr)
   {
      // If we found a row element, cast it and get its index
      auto rowElement = dynamic_cast<Rml::Controls::ElementDataGridRow*>(target);
      if(rowElement != nullptr)
      {
         int choiceIndex = rowElement->GetParentRelativeIndex();
         m_controller.choiceSelected(choiceIndex);
      }
   }
}
