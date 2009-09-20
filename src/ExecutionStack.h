#ifndef __EXECUTION_STACK_H_
#define __EXECUTION_STACK_H_

#include "Singleton.h"

class GameState;

/**
 * The heart of the game's execution.
 * Holds different states of the game (Title Screen, Tile Engine, etc.) 
 * and allows for easy change of state.
 * Main functionality is calling logic and draw, and destroying finished states in the execute() function.
 *
 * Note: This class is a singleton.
 *
 * @author Noam Chitayat
 */
class ExecutionStack : public Singleton<ExecutionStack>
{
   /**
    * Holds GameState objects in a linked list structure for easy access by the ExecutionStack.
    *
    * @author Noam Chitayat
    */
   struct StateNode
   {
      /** The game state in this node */
      GameState* state;

      /** The previous state in the stack */
      StateNode* prevState;

      /** Constructor. */
      StateNode(GameState* gs, StateNode* prev) : state(gs), prevState(prev)
      {
      }
   };

   /** The state stack (stored as a linked list) */
   StateNode* currentState;

   /**
    * Remove the most recent state pushed on the stack
    *
    * @return false iff there is no remaining state (stack is empty)
    */
   bool popState();

   protected:
      /**
       * Singleton constructor.
       * Initializes an empty execution stack.
       */
      void initialize();

      /**
       * Singleton destructor.
       *
       * Destroys all remaining states.
       */
      void finish();
       
   public:
      /**
       * Pushes (and activates) a new game state.
       * After this method call, newState is responsible
       * for all drawing and logic steps until it is finished
       * or another state is pushed
       *
       * @param newState the new state of the game
       */
      void pushState(GameState* newState);

      /**
       * Execute the game loop.
       * Step through the state logic. If the logic returns true
       * then the state is not ready to terminate, so run its draw step.
       * Otherwise, pop the stack and activate the next most recent state.
       * Keep going until there are no more states, and then quit.
       */
      void execute();
};

#endif
