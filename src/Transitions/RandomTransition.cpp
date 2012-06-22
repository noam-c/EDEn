/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "RandomTransition.h"
#include "ExecutionStack.h"
#include "GameState.h"
#include "FadeState.h"
#include "SpinState.h"

// For rand()
#include <cstdlib>

const int NUM_TRANSITIONS = 2;

GameState* RandomTransition::create(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength)
{
  int randomNumber = rand() % NUM_TRANSITIONS;
  GameState* transition;
  switch(randomNumber)
  {
    case 0: transition = new FadeState(executionStack, oldState, newState, transitionLength); break;
    case 1: transition = new SpinState(executionStack, oldState, newState, transitionLength); break;
  }
  return transition;
}
