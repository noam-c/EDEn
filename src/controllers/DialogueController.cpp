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

#define DEBUG_FLAG DEBUG_DIA_CONTR

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
   m_timeSinceLastCharacterAdded(MILLISECONDS_PER_LETTER),
   m_charsToShow(0),
   m_fastMode(false)
{
}

void DialogueController::initialize(Scheduler& scheduler, std::weak_ptr<DialogueBox> dialogueBox)
{
   m_mainDialogue = std::move(dialogueBox);
   clearDialogue();

   scheduler.start(std::make_shared<DialogueCoroutine>(*this));
}

void DialogueController::addLine(DialogueEntryType type, const std::string& text, const DialogueChoiceList& choices, const std::shared_ptr<Task>& task)
{
   bool currentlyHasDialogue = hasDialogue();
   m_dialogueQueue.emplace(type, text, choices, task);

   if(!currentlyHasDialogue)
   {
      updateDialogueBox(true /*hasCurrentLineChanged*/);
   }
}

void DialogueController::narrate(const std::string& text, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices)
{
   addLine(NARRATE, text, choices, task);
}

void DialogueController::say(const std::string& text, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices)
{
   addLine(SAY, text, choices, task);
}

void DialogueController::setFastModeEnabled(bool enabled)
{
   if(!m_fastMode)
   {
      m_timeSinceLastCharacterAdded = -1;
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

   DialogueEntry& currEntry = m_dialogueQueue.front();
   // See if we ran over any embedded scripts that we should execute
   unsigned int openIndex, closeIndex;
   if(currEntry.getNextBracketPair(openIndex, closeIndex))
   {
      // If there is a bracket pair coming up and we're past the point of the
      // open bracket, then extract the script string and run it
      if(openIndex <= m_charsToShow)
      {
         m_charsToShow = openIndex;
         std::string script = currEntry.removeNextScriptString();
         m_scriptEngine.runScriptString(script);
      }
   }

   m_charsToShow = std::min(m_charsToShow, currEntry.text.size());
   updateDialogueBox();
}

/**
 * Update the dialogue box to reflect the current line of dialogue.
 */
void DialogueController::updateDialogueBox(bool hasCurrentLineChanged)
{
   auto dialogueBox = m_mainDialogue.lock();
   if (dialogueBox)
   {
      if(hasCurrentLineChanged)
      {
         dialogueBox->onDialogueChanged();
      }
      else
      {
         dialogueBox->onDialogueAdvanced();
      }
   }
}

bool DialogueController::isCurrentLineComplete() const
{
   return hasDialogue() && m_charsToShow >= m_dialogueQueue.front().text.size();
}

bool DialogueController::hasDialogue() const
{
   return !m_dialogueQueue.empty();
}

void DialogueController::clearDialogue()
{
   m_timeSinceLastCharacterAdded = getMillisecondsPerCharacter();
   m_charsToShow = 0;

   if(!m_dialogueQueue.empty())
   {
      m_dialogueQueue.pop();
   }

   updateDialogueBox(true /*hasCurrentLineChanged*/);
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

void DialogueController::choiceSelected(int choiceIndex)
{
   if(!hasDialogue() || !isCurrentLineComplete())
   {
      return;
   }

   if(m_dialogueQueue.front().choiceSelected(choiceIndex))
   {
      clearDialogue();
   }
}

bool DialogueController::nextLine()
{
   if(!hasDialogue() || !isCurrentLineComplete() || hasChoices())
   {
      return false;
   }

   // If the dialogue is finished, clear the dialogue box and
   // move on to the next line
   clearDialogue();

   return true;
}

bool DialogueController::resume(long timePassed)
{
   if(hasDialogue() && !isCurrentLineComplete())
   {
      m_timeSinceLastCharacterAdded -= timePassed;
      while(m_timeSinceLastCharacterAdded < 0)
      {
         m_timeSinceLastCharacterAdded += getMillisecondsPerCharacter();
         ++m_charsToShow;
      }

      advanceDialogue();
   }

   return false;
}

bool DialogueController::hasChoices() const
{
   return hasDialogue() && !m_dialogueQueue.front().choices.empty();
}

const DialogueChoiceList& DialogueController::getCurrentChoices() const
{
   return m_dialogueQueue.front().choices;
}

std::string DialogueController::getTextToShow() const
{
   if (!hasDialogue())
   {
      return "";
   }

   auto& currLineText = m_dialogueQueue.front().text;
   return currLineText.substr(0, m_charsToShow);
}

DialogueEntryType DialogueController::getEntryType() const
{
   return hasDialogue() ?
      m_dialogueQueue.front().type :
      DialogueEntryType::SAY;
}
