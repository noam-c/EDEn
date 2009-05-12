#ifndef __DIALOGUE_CONTROL_H_
#define __DIALOGUE_CONTROL_H_

#include <queue>
#include <string>

#include "TicketId.h"

namespace edwt
{  class Container;
   class TextBox;
};

class ScriptEngine;

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
 * (such as rabbling crowds or unstable machinery). Currently, though, we are not supporting this capability.
 *
 * @author Noam Chitayat
 */
class DialogueController
{  /** Abstract the implementation of the dialogue boxes */
   typedef edwt::TextBox DialogueBox;

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

      /** The ticket waiting on this particular line of dialogue */
      TicketId ticket;

      /** Constructor. */
      Line(LineType type, std::string dialogue, TicketId ticket) 
                    : type(type), dialogue(dialogue), ticket(ticket)
      {}
   };

   /** The queue to hold all the pending dialogue sequences. */
   std::queue<Line*> lineQueue;

   /** The topmost container that will contain the dialogue boxes */
   edwt::Container* top;

   /** Main dialogue box for speech or narration */
   DialogueBox* mainDialogue;

   /** How much time has passed since the current line of dialogue began */
   long dialogueTime;

   /** The current line of dialogue */
   Line* currLine;

   /** The scripting engine that will be interfacing with this controller */
   ScriptEngine* scriptEngine;

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
    * Set the current line to be a narration or speech;
    * alter the dialogue box accordingly.
    *
    * @param type The type of line that will be shown.
    */
   void setDialogue(LineType type);

   /**
    * Enqueue a line of speech in the dialogue box.
    * If there is already a line being spoken, append the new speech.
    *
    * @param type The type of line that will be enqueued.
    * @param speech The speech to enqueue in the dialogue controller. 
    * @param ticket The ticket number to be signalled when the line is finished
    */
   void addLine(LineType type, const char* speech, TicketId ticket);

   /**
    * Clears any dialogue currently being displayed onscreen.
    */
   void clearDialogue();

   public:

      /**
       * Constructor.
       *
       * @param top The top-level widget container of the current state.
       */
      DialogueController(ScriptEngine* scriptEngine, edwt::Container* top);

      /**
       * Clears a finished line of dialogue from the screen and loads the next
       * line in the lineQueue for display on screen.
       */
      bool nextLine();

      /**
       * Enqueue a line of speech said by a character.
       *
       * @param speech The dialogue that will be said.
       * @param ticket The ticket number of this speech instruction
       */
      void say(const char* speech, TicketId ticket);
   
      /**
       * Enqueue a line of speech narrated or thought by a character.
       *
       * @param speech The dialogue that will be narrated.
       * @param ticket The ticket number of this narration instruction
       */
      void narrate(const char* speech, TicketId ticket);

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
