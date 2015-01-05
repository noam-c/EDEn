/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef BLEND_STATE_H
#define BLEND_STATE_H

#include <memory>

#include "TransitionState.h"

/**
 * A transition that slowly blends in the new state while
 * simultaneously fading out the old state, creating a smooth
 * and simple transition between the two states' screens.
 *
 * @author Noam Chitayat
 */
class BlendState : public TransitionState
{
   protected:
      /**
       * Draws a blend between the old and new state.
       */
      void draw();

   public:
      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack managing this transition.
       * @param oldState The state that is being transitioned away from.
       * @param newState The state that is being transitioned to.
       * @param transitionLength The length (in milliseconds) of the transition.
       */
      BlendState(GameContext& gameContext, std::shared_ptr<GameState> oldState, std::shared_ptr<GameState> newState, long transitionLength = 1000);

      /**
       * Destructor.
       */
      ~BlendState() = default;
};

#endif
