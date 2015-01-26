/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef TRANSITION_STATE_H
#define TRANSITION_STATE_H

#include <memory>

#include "GameState.h"
#include "Transition.h"

class TransitionState : public GameState
{
   std::shared_ptr<Transition> m_transition;

   protected:
      /**
       * Update the transition to the right timeframe.
       */
      bool step(long timePassed);
      
      /**
       * Draw the transition.
       */
      void draw();

   public:
      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack managing this transition.
       * @param transition The transition to animate.
       */
      TransitionState(GameContext& gameContext, std::shared_ptr<Transition> transition);
};

#endif
