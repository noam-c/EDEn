/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
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
   m_context(context),
   m_mainDialogue(nullptr),
   m_fastMode(false)
{
   initMainDialogue();
   clearDialogue();

   scheduler.start(std::make_shared<DialogueCoroutine>(*this));
}

DialogueController::~DialogueController()
{
   if(m_dialogueBoxDocument != nullptr)
   {
      m_dialogueBoxDocument->Close();
   }
}

void DialogueController::initMainDialogue()
{
   m_dialogueBoxDocument = m_context.LoadDocument("data/gui/dialogueBox.rml");
   if(m_dialogueBoxDocument != nullptr)
   {
      m_mainDialogue = m_dialogueBoxDocument->GetElementById("textArea");
   }
}

void DialogueController::addLine(LineType type, const std::string& speech, const std::shared_ptr<Task>& task)
{
   m_lineQueue.emplace(new Line(type, speech, task));
   updateDialogueBox();
}

void DialogueController::narrate(const std::string& speech, const std::shared_ptr<Task>& task)
{
   addLine(NARRATE, speech, task);
}

void DialogueController::say(const std::string& speech, const std::shared_ptr<Task>& task)
{
   addLine(SAY, speech, task);
}

void DialogueController::updateDialogueBox()
{
   if(!m_lineQueue.empty())
   {
      m_mainDialogue->GetOwnerDocument()->Show();

      bool isSpeech = m_lineQueue.front()->type == SAY;
      m_dialogueBoxDocument->SetClass("speech", isSpeech);
      m_dialogueBoxDocument->SetClass("narration", !isSpeech);
   }
   else
   {
      m_mainDialogue->GetOwnerDocument()->Hide();
   }
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
   if(m_lineQueue.empty())
   {
      // No dialogue to advance
      return;
   }

   auto& currLine = m_lineQueue.front();
   // See if we ran over any embedded scripts that we should execute
   unsigned int openIndex, closeIndex;
   if(currLine->getNextBracketPair(openIndex, closeIndex))
   {
      // If there is a bracket pair coming up and we're past the point of the
      // open bracket, then extract the script string and run it
      if(openIndex <= m_charsToShow)
      {
         m_charsToShow = openIndex;
         std::string script = currLine->removeNextScriptString();
         m_scriptEngine.runScriptString(script);
      }
   }

   std::string dialogue = currLine->dialogue;

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
   return hasDialogue() && m_charsToShow >= m_lineQueue.front()->dialogue.size();
}

bool DialogueController::hasDialogue() const
{
   return !m_lineQueue.empty();
}

void DialogueController::clearDialogue()
{
   m_dialogueTime = getMillisecondsPerCharacter();
   m_charsToShow = 0;

   if(!m_lineQueue.empty())
   {
      m_lineQueue.pop();
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
   updateDialogueBox();

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

DialogueController::Line::Line(LineType type, const std::string& dialogue, const std::shared_ptr<Task>& task) :
   task(task),
   type(type),
   dialogue(dialogue)
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

DialogueController::Line::~Line()
{
   if(task)
   {
      task->complete();
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
