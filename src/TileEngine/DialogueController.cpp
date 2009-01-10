#include "DialogueController.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_DIA_CONTR;

DialogueController::DialogueController(gcn::Container* top) : top(top)
{  initMainDialogue();

   dialogueTime = 0;
   currLine = NULL;
}

void DialogueController::initMainDialogue()
{  mainDialogue = new DialogueBox();
   mainDialogue->setEditable(false);

   mainDialogue->setVisible(false);
   mainDialogue->setWidth(800);
   mainDialogue->setX(0);

   top->add(mainDialogue);
}

void DialogueController::narrate(const char* speech)
{  if( lineQueue.empty() )
   {  Line* narration = new Line(NARRATE, speech);
      lineQueue.push(narration);
   }
   else
   {  lineQueue.front()->dialogue.append(speech);
   }
}

void DialogueController::say(const char* speech)
{  Line* narration = new Line(SAY, speech);
//   lineQueue.push(narration);
}

void DialogueController::setDialogue(LineType type)
{  switch(type)
   {  case NARRATE:
      {  mainDialogue->setOpaque(false);
         mainDialogue->setAlignment(edwt::TextBox::CENTER);
      
         mainDialogue->setY(600/2 - mainDialogue->getHeight()/2);
         mainDialogue->setForegroundColor(gcn::Color(255,255,255));   
         mainDialogue->setVisible(true);
         break;
      }
      case SAY:
      {  mainDialogue->setOpaque(true);
         mainDialogue->setAlignment(edwt::TextBox::LEFT);
      
         mainDialogue->setHeight(100);
         mainDialogue->setY(600 - mainDialogue->getHeight());
         mainDialogue->setForegroundColor(gcn::Color(0,0,0));
         mainDialogue->setVisible(true);
      }   
   }
}

void DialogueController::showNextDialogue()
{  if(!hasDialogue() && !lineQueue.empty())
   {  currLine = lineQueue.front();
      lineQueue.pop();
      setDialogue(currLine->type);
      dialogueTime = 0;
   }
}

void DialogueController::advanceDialogue()
{  int charsToShow = dialogueTime / 100;

   std::string dialogue = currLine->dialogue;

   if(dialogue.size() <= charsToShow)
   {  charsToShow = dialogue.size();
      dialogueTime = -1;
   }

   dialogue = dialogue.substr(0, charsToShow);
   mainDialogue->setText(dialogue);
}

bool DialogueController::dialogueComplete()
{  return dialogueTime == -1;
}

bool DialogueController::hasDialogue()
{  return currLine != NULL;
}

void DialogueController::timePassed(long time)
{  if(hasDialogue() && !dialogueComplete())
   {  dialogueTime += time;
      advanceDialogue();
   }
   else if(!hasDialogue())
   {  showNextDialogue();
   }
}
