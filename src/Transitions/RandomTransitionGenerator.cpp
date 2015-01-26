/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "RandomTransitionGenerator.h"

#include "BlendState.h"
#include "FadeState.h"
#include "SpinState.h"
#include "TransitionState.h"

// For rand()
#include <cstdlib>

const int NUM_TRANSITIONS = 3;

std::shared_ptr<GameState> RandomTransitionGenerator::create(GameContext& gameContext, Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength)
{
   int randomNumber = rand() % NUM_TRANSITIONS;
   std::shared_ptr<Transition> transition;
   switch(randomNumber)
   {
      case 0: transition = std::make_shared<FadeState>(std::move(oldStateTexture), transitionLength); break;
      case 1: transition = std::make_shared<SpinState>(std::move(oldStateTexture), transitionLength); break;
      case 2: transition = std::make_shared<BlendState>(std::move(oldStateTexture), std::move(newStateTexture), transitionLength); break;
   }

   return std::make_shared<TransitionState>(gameContext, transition);
}
