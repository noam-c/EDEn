#include "DialogueController.h"
#include "TextBox.h"
#include "Container.h"
#include "ScriptEngine.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_DIA_CONTR;

DialogueController::DialogueController(edwt::Container& top, ScriptEngine& engine)
                     : scriptEngine(engine), top(top), fastMode(false), currLine(NULL)
{
   initMainDialogue();
   clearDialogue();
}

void DialogueController::initMainDialogue()
{
   mainDialogue = new DialogueBox();
   mainDialogue->setEditable(false);

   mainDialogue->setVisible(false);
   mainDialogue->setWidth(800);
   mainDialogue->setX(0);

   top.add(mainDialogue);
}

void DialogueController::addLine(LineType type, const char* speech, Task* task)
{
   Line* nextLine = new Line(type, speech, task);
   if(currLine == NULL)
   {
      currLine = nextLine;
      setDialogue(type);
   }
   else
   {
      lineQueue.push(nextLine);
   }
}

void DialogueController::narrate(const char* speech, Task* task)
{
   addLine(NARRATE, speech, task);
}

void DialogueController::say(const char* speech, Task* task)
{
   addLine(SAY, speech, task);
}

void DialogueController::setDialogue(LineType type)
{
   switch(type)
   {
      case NARRATE:
      {
         mainDialogue->setOpaque(false);
         mainDialogue->setAlignment(edwt::TextBox::CENTER);
      
         mainDialogue->setY(600/2 - mainDialogue->getHeight()/2);
         mainDialogue->setForegroundColor(gcn::Color(255,255,255));   
         mainDialogue->setVisible(true);
         break;
      }
      case SAY:
      {
         mainDialogue->setOpaque(true);
         mainDialogue->setAlignment(edwt::TextBox::LEFT);
      
         mainDialogue->setHeight(100);
         mainDialogue->setY(600 - mainDialogue->getHeight());
         mainDialogue->setForegroundColor(gcn::Color(0,0,0));
         mainDialogue->setVisible(true);
      }   
   }
}

void DialogueController::setFastModeEnabled(bool enabled)
{
   if(!fastMode)
   {
      dialogueTime = -1;
   }

   fastMode = enabled;
}

void DialogueController::advanceDialogue()
{
   // See if we ran over any embedded scripts that we should execute
   unsigned int openIndex, closeIndex;
   if(currLine->getNextBracketPair(openIndex, closeIndex))
   {
      // If there is a bracket pair coming up and we're past the point of the
      // open bracket, then extract the script string and run it
      if(openIndex <= charsToShow)
      {
         charsToShow = openIndex;
         std::string script = currLine->removeNextScriptString();
         scriptEngine.runScriptString(script);
      }
   }

   std::string dialogue = currLine->dialogue;

   // If we have run to the end of the dialogue, we show all the text
   // and signal that the associated task is done.
   if(dialogue.size() <= charsToShow)
   {
      charsToShow = dialogue.size();
      currLine->task->signal();
   }

   // Display the necessary piece of text in the text box
   dialogue = dialogue.substr(0, charsToShow);
   mainDialogue->setText(dialogue);
}

bool DialogueController::dialogueComplete()
{
   return charsToShow == currLine->dialogue.size();
}

bool DialogueController::hasDialogue()
{
   return currLine != NULL;
}

void DialogueController::clearDialogue()
{
   dialogueTime = getMillisecondsPerCharacter();
   charsToShow = 0;

   if(currLine)
   {
      delete currLine;
      currLine = NULL;
   }

   mainDialogue->setText("");
}

int DialogueController::getMillisecondsPerCharacter()
{
   int time = MILLISECONDS_PER_LETTER;
   if(fastMode)
   {
      time >>= 2;
   }

   return time;
}

bool DialogueController::nextLine()
{
   if(!hasDialogue() || !dialogueComplete())
   {
      return false;
   }

   // If the dialogue is finished, clear the dialogue box and 
   // move on to the next line
   clearDialogue();
   if(lineQueue.empty())
   {
      currLine = NULL;
   }
   else
   {
      currLine = lineQueue.front();
      lineQueue.pop();
   }

   return true;
}

bool DialogueController::resume(long timePassed)
{
   if(hasDialogue() && !dialogueComplete())
   {
      dialogueTime -= timePassed;
      while(dialogueTime < 0)
      {
         dialogueTime += getMillisecondsPerCharacter();
         ++charsToShow;
      }

      advanceDialogue();
   }

   return false;
}

DialogueController::Line::Line(LineType type, const std::string& dialogue, Task* task)
                    : type(type), dialogue(dialogue), task(task)
{
   int openIndex = 0;
   int closeIndex = 0;

   for(;;)
   {
      int nextOpenIndex = dialogue.find('<', openIndex+1);
      int nextCloseIndex = dialogue.find('>', closeIndex+1);

      if(nextOpenIndex < 0)
      {
         if(nextCloseIndex >= 0)
         {
            T_T("Extra '>' character detected in dialogue line."
                " Please balance your dialogue script brackets (< and >).");
         }

         break;
      }
      else if(nextCloseIndex < 0)
      {
         T_T("Found '<' without matching '>' in dialogue line."
            " Please balance your dialogue script brackets ('<' and '>').");
      }
      else if(nextCloseIndex < nextOpenIndex)
      {
         T_T("Found extra '>' character in dialogue line."
            " Please balance your dialogue script brackets ('<' and '>').");
      }
      else if(nextOpenIndex < closeIndex)
      {
         T_T("Found nested '<' character in dialogue line."
            " Please revise the line to remove nested brackets ('<' and '>').");
      }
      else
      {
         openIndex = nextOpenIndex;
         closeIndex = nextCloseIndex;
         openScriptBrackets.push(openIndex);
         closeScriptBrackets.push(closeIndex);
         DEBUG("Found embedded script starting at %d, ending at %d", openIndex, closeIndex); 
      }
   }
}

bool DialogueController::Line::getNextBracketPair(unsigned int& openIndex, unsigned int& closeIndex)
{
   if(openScriptBrackets.empty())
   {
      return false;
   }

   openIndex = openScriptBrackets.front();
   closeIndex = closeScriptBrackets.front();

   return true;
}

std::string DialogueController::Line::removeNextScriptString()
{
   unsigned int openIndex, closeIndex;
   getNextBracketPair(openIndex, closeIndex);

   openScriptBrackets.pop();
   closeScriptBrackets.pop();

   const int scriptLength = closeIndex - openIndex;
   std::string script = dialogue.substr(openIndex + 1, scriptLength - 1);
   dialogue.replace(openIndex, scriptLength + 1, "");

   DEBUG("Extracting script %s, leaving dialogue %s", script.c_str(), dialogue.c_str());
   return script;
}
