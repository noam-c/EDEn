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

namespace Rocket
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
   Rocket::Core::Element* m_dialogueBox;

   /** The RML element holding the dialogue box text area */
   Rocket::Core::Element* m_dialogueTextArea;

   /** The RML element holding the dialogue box choice list */
   Rocket::Core::Element* m_dialogueChoiceList;

   DialogueController& m_controller;

   ChoicesDataSource m_choicesDataSource;

   void onChoiceListClicked(Rocket::Core::Event& event);

   public:
      DialogueBox(Rocket::Core::Element* dialogueBox, DialogueController& controller);

      void onDialogueAdvanced();

      void onDialogueChanged();

      void refresh();
};

#endif
