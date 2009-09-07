#ifndef __DIALOGUE_CONTROL_H_
#define __DIALOGUE_CONTROL_H_

#include <queue>
#include <string>

#include "Thread.h"
#include "Task.h"

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
class DialogueController : public Thread
{  /** The hardcoded time-per-letter speed */
   static const int MILLISECONDS_PER_LETTER = 50;
   
   /** Abstract the implementation of the dialogue boxes */
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
   class Line
   {  /** A queue of upcoming open script brackets ('<' characters) */
      std::queue<int> openScriptBrackets;

      /** A queue of upcoming close script brackets ('>' characters) */
      std::queue<int> closeScriptBrackets;

      public:
         /** The type of line (how it should be displayed) */
         LineType type;
   
         /** The dialogue itself. */
         std::string dialogue;
   
         /** The task ID waiting on this particular line of dialogue */
         Task* task;
   
         /**
          *  Constructor. Initializes values and indexes the locations of
          *  embedded scripts in the line of dialogue for later use.
          */
         Line(LineType type, std::string dialogue, Task* task);

         /**
          *  Gets the next embdedded script bracket pair.
          *
          *  @return true iff there are embedded script brackets left in the string
          */
         bool getNextBracketPair(int& openIndex, int& closeIndex);

         /**
          *  Gets the next embedded script and removes it from the dialogue line.
          *
          *  @return the script string removed from the dialogue line
          */
         std::string removeNextScriptString();
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

   /** The script engine to call when embedded instructions are found */
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
    * @param task The ticket to be signalled when the line is finished
    */
   void addLine(LineType type, const char* speech, Task* task);

   /**
    * Clears any dialogue currently being displayed onscreen.
    */
   void clearDialogue();

   public:

      /**
       * Constructor.
       *
       * @param top The top-level widget container of the current state.
       * @param engine The scripting engine to call with embedded scripts.
       */
      DialogueController(edwt::Container* top, ScriptEngine* engine);

      /**
       * Clears a finished line of dialogue from the screen and loads the next
       * line in the lineQueue for display on screen. Only works when there is
       * dialogue and it isn't in the process of printing
       *
       * @return true iff the controller was able to respond to the nextLine request
       */
      bool nextLine();

      /**
       * Enqueue a line of speech said by a character.
       *
       * @param speech The dialogue that will be said.
       * @param task The ticket of this speech instruction
       */
      void say(const char* speech, Task* task);
   
      /**
       * Enqueue a line of speech narrated or thought by a character.
       *
       * @param speech The dialogue that will be narrated.
       * @param task The ticket of this narration instruction
       */
      void narrate(const char* speech, Task* task);

      /**
       * Signals that some amount of time has passed.
       * Tells the DialogueController how much of the current line
       * of dialogue to reveal (it is shown letter by letter over time)
       *
       * @param timePassed The number of milliseconds that has passed since the last frame.
       * @return false only, since this Thread does not end until end-of-life
       */
      bool resume(long time);

};

#endif
