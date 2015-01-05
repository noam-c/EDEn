/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "RandomTransitionGenerator.h"
#include "ExecutionStack.h"
#include "FadeState.h"
#include "SpinState.h"
#include "BlendState.h"

// For rand()
#include <cstdlib>

const int NUM_TRANSITIONS = 3;

std::shared_ptr<GameState> RandomTransitionGenerator::create(GameContext& gameContext, std::shared_ptr<GameState> oldState, std::shared_ptr<GameState> newState, long transitionLength)
{
  int randomNumber = rand() % NUM_TRANSITIONS;
  std::shared_ptr<GameState> transition;
  switch(randomNumber)
  {
     case 0: transition = std::make_shared<FadeState>(gameContext, oldState, transitionLength); break;
     case 1: transition = std::make_shared<SpinState>(gameContext, oldState, transitionLength); break;
     case 2: transition = std::make_shared<BlendState>(gameContext, oldState, newState, transitionLength); break;
  }

  return transition;
}
