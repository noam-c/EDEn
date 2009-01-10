#ifndef __DIALOGUE_CONTROL_H_
#define __DIALOGUE_CONTROL_H_

#include "TextBox.h"
#include <vector>
#include <queue>

/**
 * The dialogue controller controls all of the dialogue boxes that hold
 * character speech, thought, or narration of any kind.
 * When there is dialogue, it appears letter by letter at a predetermined
 * pace. Ideally, this pace would be chosen/changed by the user, but it is
 * HARDCODED for now. 
 *
 * If there is already text in the main dialogue box that is not finished,
 * any new text is enqueued in preparation.
 * Eventually, we may support having "mini-boxes" for many small utterances around the screen
 * (such as rabbling crowds or unstable machinery).
 *
 * @author Noam Chitayat
 */
class DialogueController
{  /** Abstract the implementation of the dialogue boxes */
   typedef edwt::TextBox DialogueBox;

   /** As long as dialogue instructions are hardcoded, this is necessary */
   friend class TileEngine;

   /**
    * There are two kinds of dialogues (for now); speech and narration/thought.
    */
   enum LineType
   {  /** Dialogue is narrated in center screen */
      NARRATE,
      /** Dialogue is spoken to the player via bottom dialogue box */
      SAY
   };

   /**
    * A line of dialogue is a string of dialogue and a LineType
    * It determines what is said and how.
    *
    * @author Noam Chitayat
    */
   struct Line
   {  /** The type of line (how it should be displayed) */
      LineType type;

      /** The dialogue itself. */
      std::string dialogue;

      /** Constructor. */
      Line(LineType type, std::string dialogue) 
                    : type(type), dialogue(dialogue)
      {}
   };

   /** The topmost container that will contain the dialogue boxes */
   gcn::Container* top;

   /** Main dialogue box for speech or narration */
   DialogueBox* mainDialogue;

   /** Main queue for speech lines to appear in the main dialogue box */
   std::queue<Line*> lineQueue;


   /** How much time has passed since the current line of dialogue began */
   long dialogueTime;

   /** The current line of dialogue */
   Line* currLine;

   /**
    * Enqueue a line of speech said by a character.
    *
    * @param speech The dialogue that will be said.
    */
   void say(const char* speech);

   /**
    * Enqueue a line of speech narrated or thought by a character.
    *
    * @param speech The dialogue that will be narrated.
    */
   void narrate(const char* speech);

   /**
    * Initialize the main dialogue box.
    */
   void initMainDialogue();

   /**
    * @return true iff there is currently a line of dialogue being shown.
    */
   bool hasDialogue();

   /**
    * @return true iff the current line of dialogue is finished writing to screen
    */
   bool dialogueComplete();

   /**
    * Refresh the dialogue box to show enough letters on the screen for the amount of time passed.
    */
   void advanceDialogue();

   /**
    * If there is dialogue enqueued, display it as the current line.
    */
   void showNextDialogue();

   /**
    * Set the current line to be a narration or speech;
    * alter the dialogue box accordingly.
    *
    * @param type The type of line that will be shown.
    */
   void setDialogue(LineType type);

   public:

      /**
       * Constructor.
       *
       * @param top The top-level widget container of the current state.
       */
      DialogueController(gcn::Container* top);

      /**
       * Signals that some amount of time has passed.
       * Tells the DialogueController how much of the current line
       * of dialogue to reveal (it is shown letter by letter over time)
       *
       * @param time The number of milliseconds that has passed since the last frame.
       */
      void timePassed(long time);
};

#endif
