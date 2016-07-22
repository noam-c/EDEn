/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef TRANSITION_STATE_H
#define TRANSITION_STATE_H

#include <memory>
#include <type_traits>

#include "GameState.h"

class Transition;

class TransitionState final : public GameState
{
   std::unique_ptr<Transition> m_transition;

   protected:
      /**
       * Update the transition to the right timeframe.
       */
      bool step(long timePassed) override;
      
      /**
       * Draw the transition.
       */
      void draw() override;

   public:
      /**
       * Constructor.
       *
       * @param gameContext The context containing the execution stack managing this transition.
       * @param transition The transition to animate.
       */
      TransitionState(GameContext& gameContext, std::unique_ptr<Transition>&& transition);

      ~TransitionState();

      template <typename TransitionType, typename ... TransitionArgs> static std::shared_ptr<TransitionState> makeTransition(GameContext& gameContext, TransitionArgs&& ... args)
         {
            static_assert(std::is_base_of<Transition, TransitionType>::value, "Cannot create a TransitionState with anything other than a Transition subclass.");

            std::unique_ptr<TransitionType> transition(new TransitionType(std::forward<TransitionArgs>(args)...));
            return std::make_shared<TransitionState>(gameContext, std::move(transition));
         }
};

#endif
