/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "DialogueController.h"

#include <Rocket/Core.h>

#include "Scheduler.h"
#include "ScriptEngine.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_DIA_CONTR;

DialogueController::DialogueCoroutine::DialogueCoroutine(DialogueController& controller) :
   m_dialogueController(controller)
{
}

bool DialogueController::DialogueCoroutine::resume(long timePassed)
{
   return m_dialogueController.resume(timePassed);
}

DialogueController::DialogueController(Rocket::Core::Context& context, Scheduler& scheduler, ScriptEngine& engine) :
   m_scriptEngine(engine),
   m_scheduler(scheduler),
   m_context(context),
   m_mainDialogue(NULL),
   m_fastMode(false),
   m_currLine(NULL)
{
   initMainDialogue();
   clearDialogue();

   scheduler.start(new DialogueCoroutine(*this));
}

DialogueController::~DialogueController()
{
   if(m_dialogueBoxDocument != NULL)
   {
      m_dialogueBoxDocument->Close();
   }
}

void DialogueController::initMainDialogue()
{
   m_dialogueBoxDocument = m_context.LoadDocument("data/gui/dialogueBox.rml");
   if(m_dialogueBoxDocument != NULL)
   {
      m_mainDialogue = m_dialogueBoxDocument->GetElementById("textArea");
   }
}

void DialogueController::addLine(LineType type, const std::string& speech, Task* task)
{
   Line* nextLine = new Line(type, speech, task);
   if(m_currLine == NULL)
   {
      m_currLine = nextLine;
      setDialogue(type);
   }
   else
   {
      m_lineQueue.push(nextLine);
   }
}

void DialogueController::narrate(const std::string& speech, Task* task)
{
   addLine(NARRATE, speech, task);
}

void DialogueController::say(const std::string& speech, Task* task)
{
   addLine(SAY, speech, task);
}

void DialogueController::setDialogue(LineType type)
{
   m_mainDialogue->GetOwnerDocument()->Show();

   bool isSpeech = type == SAY;
   m_dialogueBoxDocument->SetClass("speech", isSpeech);
   m_dialogueBoxDocument->SetClass("narration", !isSpeech);
}

void DialogueController::setFastModeEnabled(bool enabled)
{
   if(!m_fastMode)
   {
      m_dialogueTime = -1;
   }

   m_fastMode = enabled;
}

void DialogueController::advanceDialogue()
{
   // See if we ran over any embedded scripts that we should execute
   unsigned int openIndex, closeIndex;
   if(m_currLine->getNextBracketPair(openIndex, closeIndex))
   {
      // If there is a bracket pair coming up and we're past the point of the
      // open bracket, then extract the script string and run it
      if(openIndex <= m_charsToShow)
      {
         m_charsToShow = openIndex;
         std::string script = m_currLine->removeNextScriptString();
         m_scriptEngine.runScriptString(script, m_scheduler);
      }
   }

   std::string dialogue = m_currLine->dialogue;

   // If we have run to the end of the dialogue, we show all the text.
   if(m_charsToShow >= dialogue.size())
   {
      m_charsToShow = dialogue.size();
   }

   m_dialogueBoxDocument->SetClass("dialogueComplete", dialogueComplete());
   
   // Display the necessary piece of text in the text box
   dialogue = dialogue.substr(0, m_charsToShow);
   m_mainDialogue->SetInnerRML(dialogue.c_str());
}

bool DialogueController::dialogueComplete() const
{
   return m_charsToShow == m_currLine->dialogue.size();
}

bool DialogueController::hasDialogue() const
{
   return m_currLine != NULL;
}

void DialogueController::clearDialogue()
{
   m_dialogueTime = getMillisecondsPerCharacter();
   m_charsToShow = 0;

   if(m_currLine)
   {
      // Signal that the associated task is done before
      // clearing out the completed line.
      m_currLine->task->signal();
      delete m_currLine;
      m_currLine = NULL;
   }

   m_mainDialogue->SetInnerRML("");
}

int DialogueController::getMillisecondsPerCharacter() const
{
   int time = MILLISECONDS_PER_LETTER;
   if(m_fastMode)
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
   if(m_lineQueue.empty())
   {
      m_currLine = NULL;
      m_mainDialogue->GetOwnerDocument()->Hide();
   }
   else
   {
      m_currLine = m_lineQueue.front();
      m_lineQueue.pop();
   }

   return true;
}

bool DialogueController::resume(long timePassed)
{
   if(hasDialogue() && !dialogueComplete())
   {
      m_dialogueTime -= timePassed;
      while(m_dialogueTime < 0)
      {
         m_dialogueTime += getMillisecondsPerCharacter();
         ++m_charsToShow;
      }

      advanceDialogue();
   }

   return false;
}

DialogueController::Line::Line(LineType type, const std::string& dialogue, Task* task) :
   type(type),
   dialogue(dialogue),
   task(task)
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
         m_openScriptBrackets.push(openIndex);
         m_closeScriptBrackets.push(closeIndex);
         DEBUG("Found embedded script starting at %d, ending at %d", openIndex, closeIndex); 
      }
   }
}

bool DialogueController::Line::getNextBracketPair(unsigned int& openIndex, unsigned int& closeIndex) const
{
   if(m_openScriptBrackets.empty())
   {
      return false;
   }

   openIndex = m_openScriptBrackets.front();
   closeIndex = m_closeScriptBrackets.front();

   return true;
}

std::string DialogueController::Line::removeNextScriptString()
{
   unsigned int openIndex, closeIndex;
   getNextBracketPair(openIndex, closeIndex);

   m_openScriptBrackets.pop();
   m_closeScriptBrackets.pop();

   const int scriptLength = closeIndex - openIndex;
   std::string script = dialogue.substr(openIndex + 1, scriptLength - 1);
   dialogue.replace(openIndex, scriptLength + 1, "");

   DEBUG("Extracting script %s, leaving dialogue %s", script.c_str(), dialogue.c_str());
   return script;
}
