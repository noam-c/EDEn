/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "RandomTransitionGenerator.h"
#include "ExecutionStack.h"
#include "FadeState.h"
#include "SpinState.h"
#include "BlendState.h"

// For rand()
#include <cstdlib>

const int NUM_TRANSITIONS = 3;

GameState* RandomTransitionGenerator::create(GameContext& gameContext, GameState* oldState, GameState* newState, long transitionLength)
{
  int randomNumber = rand() % NUM_TRANSITIONS;
  GameState* transition = nullptr;
  switch(randomNumber)
  {
    case 0: transition = new FadeState(gameContext, oldState, transitionLength); break;
    case 1: transition = new SpinState(gameContext, oldState, transitionLength); break;
    case 2: transition = new BlendState(gameContext, oldState, newState, transitionLength); break;
  }

  return transition;
}
