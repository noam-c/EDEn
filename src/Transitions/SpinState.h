#ifndef SPIN_STATE_H
#define SPIN_STATE_H

#include <memory>

#include "TransitionState.h"

/**
 * A transition that creates a spin and shrink effect for the previous transition.
 *
 * @author Bobby Richter
 */
class SpinState : public TransitionState
{
   protected:
      /**
       * Draws the old state, rotated and scaled for transitional effect.
       */
      void draw();

   public:
      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack managing this transition.
       * @param oldState The state that is being faded out.
       * @param transitionLength The length (in milliseconds) of the transition.
       */
      SpinState(GameContext& gameContext, Texture&& oldStateTexture, long transitionLength = 1000);

      /**
       * Destructor.
       */
      ~SpinState() = default;
};

#endif
