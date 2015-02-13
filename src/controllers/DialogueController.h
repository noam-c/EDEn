/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef DIALOGUE_CONTROLLER_H
#define DIALOGUE_CONTROLLER_H

#include <memory>
#include <queue>
#include <string>

#include "Coroutine.h"
#include "DialogueEntry.h"

class DialogueBox;
class ScriptEngine;
class Task;

/**
 * The dialogue controller controls all of the dialogue boxes that hold
 * character speech, thought, or narration of any kind.
 * When there is dialogue, it appears letter by letter at a predetermined
 * pace. Ideally, this pace would be chosen/changed by the user, but it is
 * HARDCODED for now until an options menu is implemented.
 *
 * If there is already text in the main dialogue box that is not finished,
 * any new text is enqueued in preparation.
 * Eventually, we may support having "mini-boxes" for many small utterances around the screen
 * (such as rabbling crowds or unstable machinery). Currently, though, we are not supporting this capability.
 *
 * @author Noam Chitayat
 */
class DialogueController
{
   /** The HARDCODED time-per-letter speed */
   static const int MILLISECONDS_PER_LETTER = 100;

   class DialogueCoroutine : public Coroutine
   {
      DialogueController& m_dialogueController;

      public:
         /**
          * Constructor.
          *
          * @param dialogueController The dialogue controller that this coroutine should control.
          */
         DialogueCoroutine(DialogueController& dialogueController);

         /**
          * Resumes the associated dialogue controller.
          *
          * @param timePassed The number of milliseconds that has passed since the last frame.
          *
          * @return the result of the parent dialogue controller's resumption
          */
         bool resume(long timePassed) override;
   };

   /** The queue to hold all the pending dialogue sequences. */
   std::queue<DialogueEntry> m_dialogueQueue;

   /** The script engine to call when embedded instructions are found */
   ScriptEngine& m_scriptEngine;

   /** Main dialogue box for speech or narration */
   std::weak_ptr<DialogueBox> m_mainDialogue;

   /**
    * How much time since a letter was added to the screen's dialogue box
    * from the current line.
    */
   int m_timeSinceLastCharacterAdded;

   /**
    * The number of characters that should be placed on the screen.
    */
   size_t m_charsToShow;

   /**
    * True iff the user has indicated that dialogue should flow more quickly.
    */
   bool m_fastMode;

   /**
    * Initialize the main dialogue box.
    */
   void initMainDialogue();

   /**
    * Refresh the dialogue box to show enough letters on the screen for the amount of time passed.
    */
   void advanceDialogue();

   /**
    * Update the dialogue box to reflect the current line of dialogue.
    */
   void updateDialogueBox(bool hasCurrentLineChanged = false);

   /**
    * Enqueue a line of speech in the dialogue box.
    * If there is already a line being spoken, append the new speech.
    *
    * @param type The type of line that will be enqueued.
    * @param text The speech to enqueue in the dialogue controller.
    * @param task The ticket to be signalled when the line is finished
    */
   void addLine(DialogueEntryType type, const std::string& text, const DialogueChoiceList& choices, const std::shared_ptr<Task>& task);

   /**
    * Clears any dialogue currently being displayed onscreen.
    */
   void clearDialogue() noexcept;

   /**
    * @return The amount of time (in milliseconds) to wait before
    *         adding another letter.
    */
   int getMillisecondsPerCharacter() const noexcept;

   public:

      /**
       * Constructor.
       *
       * @param context The Rocket context of the current state.
       * @param engine The scripting engine to call with embedded scripts.
       */
      DialogueController(ScriptEngine& engine);

      /**
       * Initializes the dialogue controller by associating it with a view object and
       * starting its execution on a given scheduler.
       *
       * @param scheduler The scheduler that will manage the dialogue controller's execution.
       * @param dialogueBox The view that will be updated with dialogue changes.
       */
      void initialize(Scheduler& scheduler, std::weak_ptr<DialogueBox> dialogueBox);

      /**
       * Clears a finished line of dialogue from the screen and loads the next
       * line in the lineQueue for display on screen. Only works when there is
       * dialogue and it isn't in the process of printing
       *
       * @return true iff the controller was able to respond to the nextLine request
       */
      bool nextLine();

      void choiceSelected(int choiceIndex);

      /**
       * @return true iff there is currently a line of dialogue being shown.
       */
      bool hasDialogue() const noexcept;

      /**
       * Enqueue a line of speech said by a character.
       *
       * @param speech The dialogue that will be said.
       * @param task The task associated with this speech instruction.
       */
      void say(const std::string& speech, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices);

      /**
       * Enqueue a line of speech narrated or thought by a character.
       *
       * @param speech The dialogue that will be narrated.
       * @param task The task associated with this narration instruction
       */
      void narrate(const std::string& speech, const std::shared_ptr<Task>& task, const DialogueChoiceList& choices);

      /**
       * Enables or disables fast mode.
       *
       * @param enabled true iff fast mode should be turned on.
       */
      void setFastModeEnabled(bool enabled);

      /**
       * Signals that some amount of time has passed.
       * Tells the DialogueController how much of the current line
       * of dialogue to reveal (it is shown letter by letter over time)
       *
       * @param timePassed The number of milliseconds that has passed since the last frame.
       *
       * @return false only, since this Coroutine does not end until end-of-life
       */
      bool resume(long timePassed);

      /**
       * @return true iff the current line of dialogue is finished writing to screen
       */
      bool isCurrentLineComplete() const noexcept;

      /**
       * @return true iff the current line of dialogue has one or more choices.
       */
      bool hasChoices() const;

      /**
       * @return the choice list to show in a dialogue display.
       */
      const DialogueChoiceList& getCurrentChoices() const;

      /**
       * @return the string to show in a dialogue display.
       */
      std::string getTextToShow() const;

      /**
       * @return the type of the line currently on display.
       */
      DialogueEntryType getEntryType() const;
};

#endif
