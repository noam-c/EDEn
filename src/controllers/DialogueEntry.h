#ifndef DIALOGUE_LINE_H
#define DIALOGUE_LINE_H

#include <memory>
#include <queue>
#include <string>

#include "Task.h"

/**
 * There are two ways to display dialogue (for now); speech and narration/thought.
 */
enum DialogueEntryType
{
   /** Dialogue is narrated in center screen */
   NARRATE,

   /** Dialogue is spoken to the player via bottom dialogue box */
   SAY
};

/**
 * A line of dialogue contains everything needed to determine what is said and how it is said.
 *
 * @author Noam Chitayat
 */
class DialogueEntry
{
   /** A queue of upcoming open script brackets ('<' characters) */
   std::queue<int> m_openScriptBrackets;

   /** A queue of upcoming close script brackets ('>' characters) */
   std::queue<int> m_closeScriptBrackets;

   /** The task ID waiting on this particular line of dialogue */
   std::shared_ptr<Task> task;

   public:
      /** The type of line (how its text should be displayed) */
      DialogueEntryType type;

      /** The text in the line. */
      std::string text;

      /**
       *  Constructor. Initializes values and indexes the locations of
       *  embedded scripts in the line of dialogue for later use.
       */
      DialogueEntry(DialogueEntryType type, const std::string& text, const std::shared_ptr<Task>& task);

      ~DialogueEntry();

      /**
       *  Gets the next embdedded script bracket pair.
       *
       *  @return true iff there are embedded script brackets left in the string
       */
      bool getNextBracketPair(unsigned int& openIndex, unsigned int& closeIndex) const;

      /**
       *  Gets the next embedded script and removes it from the dialogue line.
       *
       *  @return the script string removed from the dialogue line
       */
      std::string removeNextScriptString();
};

#endif

