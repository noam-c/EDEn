/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef TRANSITION_STATE_H
#define TRANSITION_STATE_H

#include <memory>
#include <tuple>

#include "GameState.h"
#include "Texture.h"

class TransitionState : public GameState
{
   protected:
      /** The total time elapsed since the transition began. */
      long m_totalTime;

      /** The length of time that the transition will take to complete. */
      const long m_transitionLength;

      /** The progress (value between 0 and 1) through the transition. */
      double m_progress;

      /** A texture for the old state. */
      Texture m_oldStateTexture;

      /** A texture for the new state. */
      Texture m_newStateTexture;

      /**
       * Update the transition to the right timeframe.
       */
      virtual bool step(long timePassed);

      /**
       * Draw the transition.
       */
      virtual void draw() = 0;

   public:
      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack managing this transition.
       * @param stateName The identifier of the transition state.
       * @param oldState The old state that the transition will be moving from.
       * @param transitionLength An optional length (in milliseconds) for the transition.
       */
      TransitionState(GameContext& gameContext, const std::string& stateName, Texture&& oldStateTexture, long transitionLength = 1000);
   
      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack managing this transition.
       * @param stateName The identifier of the transition state.
       * @param oldState The old state that the transition will be moving from.
       * @param newState The new state that the transition will be moving to.
       * @param transitionLength An optional length (in milliseconds) for the transition.
       */
      TransitionState(GameContext& gameContext, const std::string& stateName, Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength = 1000);

      /**
       * Destructor.
       */
      virtual ~TransitionState() = 0;
};

#endif
