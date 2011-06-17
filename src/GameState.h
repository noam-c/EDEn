#ifndef __GAME_STATE_H_
#define __GAME_STATE_H_

namespace edwt
{
   class Container;
};

union SDL_Event;

/**
 * This class abstractly represents a potential state for the game to enter
 * States can include "Title Screen", "Battle", "Field" and more
 * Each GameState has a step function to perform state logic and a 
 * draw function to perform graphics work
 *
 * Game states are pushed onto the ExecutionStack, which executes the most
 * recent state of the game by causing the state to step, then draw to screen.
 *
 * The activate method is called by the Execution Stack whenever
 * the state reaches the top of the stack.
 *
 * @author Noam Chitayat
 */
class GameState
{
   protected:
      /** The container for any GUI widgets used by the state. */
      edwt::Container* top;

      /** True iff the container was created internally (and thus should also be destroyed internally) */
      bool internalContainer;

      /**
       * Set true to signal the state logic to terminate so that the state is destroyed.
       */
      bool finished;

      /**
       * Constructor.
       *
       * Initializes the top-level GUI widget container
       */
      GameState();

      /**
       * Constructor.
       *
       * Initializes the state with an existing top-level container.
       */
      GameState(edwt::Container* container);

      /**
       * Runs the state's logic processing
       *
       * @return true iff the state is not finished
       */
      virtual bool step() = 0;

      /**
       * Does common event handling that is required across all game states.
       *
       * @param event The event to handle.
       */
      void handleEvent(SDL_Event& event);

      /**
       * Runs the state's graphic and interface processing.
       * Afterwards, draws widgets, flips the buffer.
       */
      virtual void draw() = 0;

   public:
      /**
       * State activation called every time this state is found at the top of the execution stack.
       * In other words, activate is always called before this state takes control of the game loop.
       * Currently, all game state activations, at the very least, change the top level Widget container.
       */
      virtual void activate();

      virtual bool advanceFrame();

      virtual void drawFrame();

      /**
       * Destructor.
       */
      virtual ~GameState();
};

#endif
