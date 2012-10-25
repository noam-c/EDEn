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
      long startTime;
      long transitionLength;
      double progress;

      ScreenTexture oldStateTexture;
      ScreenTexture newStateTexture;

      virtual bool step();
      virtual void draw() = 0;

   public:
      TransitionState(ExecutionStack& executionStack, const std::string& stateName, GameState* oldState, GameState* newState, long transitionLength = 1000);
      virtual ~TransitionState() = 0;
};

#endif
