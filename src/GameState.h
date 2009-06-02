#ifndef __GAME_STATE_H_
#define __GAME_STATE_H_

namespace edwt
{  class Container;
};

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
{   protected:
        /** The container for any GUI widgets used by the state. */
        edwt::Container* top;

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

    public:
        /**
         * State activation called every time this state is found at the top of the execution stack.
         * In other words, activate is always called before this state takes control of the game loop.
         * Currently, all game state activations, at the very least, change the top level Widget container.
         */
        virtual void activate();

        /**
         * Runs the state's logic processing
         *
         * @return true iff the state is not finished
         */
        virtual bool step();


        /**
         * Runs the state's graphic and interface processing
         */
        virtual void draw();
};

#endif
