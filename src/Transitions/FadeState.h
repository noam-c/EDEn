/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef FADE_STATE_H
#define FADE_STATE_H

#include "TransitionState.h"

/**
 * A transition that slowly fades out the old state,
 * allowing it to completely fade to black before displaying
 * the new state.
 */
class FadeState : public TransitionState
{
   protected:
      /**
       * Draws a faded version of the old state.
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
      FadeState(GameContext& gameContext, GameState* oldState, long transitionLength = 1000);

      /**
       * Destructor.
       */
      ~FadeState();
};

#endif
