#ifndef SPIN_TRANSITION_H
#define SPIN_TRANSITION_H

#include <memory>

#include "Transition.h"

/**
 * A transition that creates a spin and shrink effect for the previous transition.
 *
 * @author Bobby Richter
 */
class SpinTransition : public Transition
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
       * @param oldStateTexture The state that is being faded out.
       * @param transitionLength The length (in milliseconds) of the transition.
       */
      SpinTransition(Texture&& oldStateTexture, long transitionLength = 1000);

      /**
       * Destructor.
       */
      ~SpinTransition() = default;
};

#endif
