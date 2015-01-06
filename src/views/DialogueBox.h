/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef DIALOGUE_BOX_H
#define DIALOGUE_BOX_H

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
   /** The RML element holding the dialogue box */
   Rocket::Core::Element* m_dialogueBox;
   
   /** The RML element holding the dialogue box text area */
   Rocket::Core::Element* m_dialogueTextArea;

   DialogueController& m_controller;
   
   public:
      DialogueBox(Rocket::Core::Element* dialogueBox, DialogueController& controller);

      void refresh();
};

#endif
