/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef FADE_TRANSITION_H
#define FADE_TRANSITION_H

#include <memory>

#include "Transition.h"

/**
 * A transition that slowly fades out the old state,
 * allowing it to completely fade to black before displaying
 * the new state.
 */
class FadeTransition final : public Transition
{
   protected:
      /**
       * Draws a faded version of the old state.
       */
      void draw() override;

   public:
      /**
       * Constructor.
       *
       * @param oldState The state that is being faded out.
       * @param transitionLength The length (in milliseconds) of the transition.
       */
      FadeTransition(Texture&& oldStateTexture, long transitionLength = 1000);

      /**
       * Destructor.
       */
      ~FadeTransition() override = default;
};

#endif
