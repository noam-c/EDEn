/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "DialogueController.h"

#include <algorithm>

#include "DialogueBox.h"
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

DialogueController::DialogueController(ScriptEngine& engine) :
   m_scriptEngine(engine),
   m_fastMode(false)
{
}

void DialogueController::initialize(Scheduler& scheduler, std::shared_ptr<DialogueBox> dialogueBox)
{
   m_mainDialogue = dialogueBox;
   clearDialogue();
   
   scheduler.start(std::make_shared<DialogueCoroutine>(*this));
}

void DialogueController::addLine(DialogueLineType type, const std::string& speech, const std::shared_ptr<Task>& task)
{
   m_lineQueue.emplace(type, speech, task);
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
   if(!hasDialogue())
   {
      // No dialogue to advance
      return;
   }

   auto& currLine = m_lineQueue.front();
   // See if we ran over any embedded scripts that we should execute
   unsigned int openIndex, closeIndex;
   if(currLine.getNextBracketPair(openIndex, closeIndex))
   {
      // If there is a bracket pair coming up and we're past the point of the
      // open bracket, then extract the script string and run it
      if(openIndex <= m_charsToShow)
      {
         m_charsToShow = openIndex;
         std::string script = currLine.removeNextScriptString();
         m_scriptEngine.runScriptString(script);
      }
   }

   m_charsToShow = std::min(m_charsToShow, currLine.text.size());
   updateDialogueBox();
}

/**
 * Update the dialogue box to reflect the current line of dialogue.
 */
void DialogueController::updateDialogueBox()
{
   auto dialogueBox = m_mainDialogue.lock();
   if (dialogueBox)
   {
      dialogueBox->refresh();
   }
}

bool DialogueController::isCurrentLineComplete() const
{
   return hasDialogue() && m_charsToShow >= m_lineQueue.front().text.size();
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

   updateDialogueBox();
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
   if(!hasDialogue() || !isCurrentLineComplete())
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
   if(hasDialogue() && !isCurrentLineComplete())
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

DialogueController::Line::Line(DialogueLineType type, const std::string& text, const std::shared_ptr<Task>& task) :
   task(task),
   type(type),
   text(text)
{
   int openIndex = 0;
   int closeIndex = 0;

   for(;;)
   {
      int nextOpenIndex = text.find('<', openIndex+1);
      int nextCloseIndex = text.find('>', closeIndex+1);

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
   std::string script = text.substr(openIndex + 1, scriptLength - 1);
   text.replace(openIndex, scriptLength + 1, "");

   DEBUG("Extracting script %s, leaving dialogue %s", script.c_str(), text.c_str());
   return script;
}

std::string DialogueController::getTextToShow() const
{
   if (!hasDialogue())
   {
      return "";
   }
   
   auto& currLineText = m_lineQueue.front().text;
   return currLineText.substr(0, m_charsToShow);
}

DialogueLineType DialogueController::getLineType() const
{
   return hasDialogue() ?
      m_lineQueue.front().type :
      DialogueLineType::SAY;
}
