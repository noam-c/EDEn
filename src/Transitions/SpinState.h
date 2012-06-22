#ifndef SPIN_STATE_H
#define SPIN_STATE_H

#include "GameState.h"
#include "ScreenTexture.h"

/**
 * A transition that creates a spin and shrink effect for the previous transition.
 *
 * @author Bobby Richter
 */
class SpinState : public GameState
{
   GameState* oldState;
   GameState* newState;

   long startTime;
   long transitionLength;
   double alpha;

   long timePassed;

   bool transitionComplete;

   ScreenTexture screenTexture;

   protected:
      bool step();
      void draw();

   public:
      SpinState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength = 1000);
      ~SpinState();
};

#endif
