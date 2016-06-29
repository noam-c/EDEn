/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef BLEND_TRANSITION_H
#define BLEND_TRANSITION_H

#include <memory>

#include "Transition.h"

/**
 * A transition that slowly blends in the new state while
 * simultaneously fading out the old state, creating a smooth
 * and simple transition between the two states' screens.
 *
 * @author Noam Chitayat
 */
class BlendTransition final : public Transition
{
   protected:
      /**
       * Draws a blend between the old and new state.
       */
      void draw() override;

   public:
      /**
       * Constructor.
       *
       * @param oldStateTexture The texture that is being transitioned away from.
       * @param newStateTexture The texture that is being transitioned to.
       * @param transitionLength The length (in milliseconds) of the transition.
       */
      BlendTransition(Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength = 1000);

      /**
       * Destructor.
       */
      ~BlendTransition() override = default;
};

#endif
