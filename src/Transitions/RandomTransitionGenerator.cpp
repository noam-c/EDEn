/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "RandomTransitionGenerator.h"

#include "BlendTransition.h"
#include "FadeTransition.h"
#include "SpinTransition.h"
#include "TransitionState.h"

// For rand()
#include <cstdlib>

const int NUM_TRANSITIONS = 3;

std::shared_ptr<GameState> RandomTransitionGenerator::create(GameContext& gameContext, Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength)
{
   int randomNumber = rand() % NUM_TRANSITIONS;
   std::shared_ptr<TransitionState> transitionState;
   switch(randomNumber)
   {
      case 0:
         transitionState = TransitionState::makeTransition<FadeTransition>(gameContext, std::move(oldStateTexture), transitionLength);
         break;
      case 1:
         transitionState = TransitionState::makeTransition<SpinTransition>(gameContext, std::move(oldStateTexture), transitionLength);
         break;
      case 2:
         transitionState = TransitionState::makeTransition<BlendTransition>(gameContext, std::move(oldStateTexture), std::move(newStateTexture), transitionLength);
         break;
   }

   return transitionState;
}
