#ifndef SPIN_STATE_H
#define SPIN_STATE_H

#include "TransitionState.h"

/**
 * A transition that creates a spin and shrink effect for the previous transition.
 *
 * @author Bobby Richter
 */
class SpinState : public TransitionState
{
   protected:
      void draw();

   public:
      SpinState(ExecutionStack& executionStack, GameState* oldState, long transitionLength = 1000);
      ~SpinState();
};

#endif
