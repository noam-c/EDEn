#include "DialogueController.h"
#include "TextBox.h"
#include "Container.h"
#include "Scheduler.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_DIA_CONTR;

DialogueController::DialogueController(edwt::Container* top, Scheduler* scheduler) 
                     : top(top), scheduler(scheduler), currLine(NULL)
{  initMainDialogue();

   clearDialogue();
}

void DialogueController::initMainDialogue()
{  mainDialogue = new DialogueBox();
   mainDialogue->setEditable(false);

   mainDialogue->setVisible(false);
   mainDialogue->setWidth(800);
   mainDialogue->setX(0);

   top->add(mainDialogue);
}

void DialogueController::addLine(LineType type, const char* speech, TaskId task)
{  Line* nextLine = new Line(type, speech, task);
   if(currLine == NULL)
   {  currLine = nextLine;
      setDialogue(type);
   }
   else
   {  lineQueue.push(nextLine);
   }
}

void DialogueController::narrate(const char* speech, TaskId task)
{  addLine(NARRATE, speech, task);
}

void DialogueController::say(const char* speech, TaskId task)
{  addLine(SAY, speech, task);
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

void DialogueController::advanceDialogue()
{  int charsToShow = dialogueTime / 100;

   std::string dialogue = currLine->dialogue;

   if(dialogue.size() <= charsToShow)
   {  charsToShow = dialogue.size();
      dialogueTime = -1;
      scheduler->taskDone(currLine->task);
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

void DialogueController::clearDialogue()
{  dialogueTime = 0;
   if(currLine) delete currLine;
   currLine = NULL;
   mainDialogue->setText("");
}

bool DialogueController::nextLine()
{  if(!hasDialogue() || !dialogueComplete())
   {  return false;
   }

   // If the dialogue is finished, clear the dialogue box and 
   // move on to the next line
   clearDialogue();
   if(lineQueue.empty())
   {  currLine = NULL;
   }
   else
   {  currLine = lineQueue.front();
      lineQueue.pop();
   }

   return true;
}

bool DialogueController::resume(long timePassed)
{  if(hasDialogue() && !dialogueComplete())
   {  dialogueTime += timePassed;
      advanceDialogue();
   }

   return false;
}
