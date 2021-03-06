/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef DIALOGUE_LINE_H
#define DIALOGUE_LINE_H

#include <memory>
#include <queue>
#include <string>
#include <vector>

#include "Task.h"

/**
 * There are two ways to display dialogue (for now); speech and narration/thought.
 */
enum class DialogueEntryType
{
   /** Dialogue is narrated in center screen */
   NARRATE,

   /** Dialogue is spoken to the player via bottom dialogue box */
   SAY
};

typedef std::vector<std::string> DialogueChoiceList;

/**
 * A line of dialogue contains everything needed to determine what is said and how it is said.
 *
 * @author Noam Chitayat
 */
class DialogueEntry final
{
   /** A queue of upcoming open script brackets ('<' characters) */
   std::queue<size_t> m_openScriptBrackets;

   /** A queue of upcoming close script brackets ('>' characters) */
   std::queue<size_t> m_closeScriptBrackets;

   /** The task waiting on this particular line of dialogue */
   std::shared_ptr<Task> m_task;

   public:
      /** The type of line (how its text should be displayed) */
      DialogueEntryType type;

      const DialogueChoiceList choices;

      /** The text in the line. */
      std::string text;

      DialogueEntry(DialogueEntryType type, const std::string& text, const std::shared_ptr<Task>& task);

      DialogueEntry(DialogueEntryType type, const DialogueChoiceList& choices, const std::shared_ptr<Task>& task);

      DialogueEntry(DialogueEntryType type, const std::string& text, const DialogueChoiceList& choices, const std::shared_ptr<Task>& task);

      void parseTextScripts();

      void finish();
      bool choiceSelected(int choiceIndex);

      /**
       *  @return the starting index of the next embedded script, or -1 if there is none
       */
      size_t getBeginningOfNextScript() const noexcept;

      /**
       *  Gets the next embedded script and removes it from the dialogue line.
       *
       *  @return the script string removed from the dialogue line
       */
      std::string removeNextScriptString();
};

#endif
