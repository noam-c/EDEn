/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef BLEND_STATE_H
#define BLEND_STATE_H

#include "GameState.h"
#include "ScreenTexture.h"

class BlendState : public GameState
{
   GameState* oldState;
   GameState* newState;

   long startTime;
   long transitionLength;
   double alpha;

   ScreenTexture oldStateTexture;
   ScreenTexture newStateTexture;

   protected:
      bool step();
      void draw();

   public:
      BlendState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength = 1000);
      ~BlendState();
};

#endif
