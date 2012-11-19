/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef TRANSITION_STATE_H
#define TRANSITION_STATE_H

#include "GameState.h"
#include "ScreenTexture.h"

class TransitionState : public GameState
{
   protected:
      /** The time (in milliseconds) that the transition began. */
      long startTime;

      /** The length of time that the transition will take to complete. */
      const long transitionLength;

      /** The progress (value between 0 and 1) through the transition. */
      double progress;

      /** A screen capture texture for the old state. */
      ScreenTexture oldStateTexture;

      /** A screen capture texture for the new state. */
      ScreenTexture newStateTexture;

      /**
       * Update the transition to the right timeframe.
       */
      virtual bool step();

      /**
       * Draw the transition.
       */
      virtual void draw() = 0;

      /**
       * Captures the given state into the given Screen Texture object.
       * NOTE: To properly capture the state, this function activates and deactivates
       * the given state.
       *
       * @param state The game state to capture
       * @param texture The texture to capture the drawn state to.
       */
      bool captureStateToTexture(GameState* state, ScreenTexture& screenTexture);

   public:
      /**
       * Constructor.
       *
       * @param executionStack The execution stack that will contain the transition state.
       * @param stateName The identifier of the transition state.
       * @param oldState The old state that the transition will be moving from.
       * @param newState The new state that the transition will be moving to.
       * @param transitionLength An optional length (in milliseconds) for the transition.
       */
      TransitionState(ExecutionStack& executionStack, const std::string& stateName, GameState* oldState, GameState* newState, long transitionLength = 1000);

      /**
       * Destructor.
       */
      virtual ~TransitionState() = 0;
};

#endif
