/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef DIALOGUE_BOX_H
#define DIALOGUE_BOX_H

#include "ChoicesDataSource.h"
#include "EdenRocketBindings.h"

class DialogueController;

namespace Rml
{
   namespace Core
   {
      class Context;
      class Element;
      class ElementDocument;
   };
};

class DialogueBox
{
   /** The event binding collection for this GUI */
   EdenRocketBindings m_bindings;

   /** The RML element holding the dialogue box */
   Rml::Core::Element* m_dialogueBox;

   /** The RML element holding the dialogue box text area */
   Rml::Core::Element* m_dialogueTextArea;

   /** The RML element holding the dialogue box choice list */
   Rml::Core::Element* m_dialogueChoiceList;

   DialogueController& m_controller;

   ChoicesDataSource m_choicesDataSource;

   void onChoiceListClicked(Rml::Core::Event& event);

   public:
      DialogueBox(Rml::Core::Element* dialogueBox, DialogueController& controller);

      void onDialogueAdvanced();

      void onDialogueChanged();

      void refresh();
};

#endif
