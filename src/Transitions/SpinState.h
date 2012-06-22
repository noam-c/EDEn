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

   long startTime;
   long transitionLength;
   double alpha;

   long timePassed;

   ScreenTexture oldStateTexture;

   protected:
      bool step();
      void draw();

   public:
      SpinState(ExecutionStack& executionStack, GameState* oldState, long transitionLength = 1000);
      ~SpinState();
};

#endif
