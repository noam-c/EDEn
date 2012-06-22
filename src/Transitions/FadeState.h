/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef FADE_STATE_H
#define FADE_STATE_H

#include "GameState.h"
#include "ScreenTexture.h"

class FadeState : public GameState
{
   GameState* oldState;

   long startTime;
   long transitionLength;
   double alpha;

   ScreenTexture oldStateTexture;

   protected:
      bool step();
      void draw();

   public:
      FadeState(ExecutionStack& executionStack, GameState* oldState, long transitionLength = 1000);
      ~FadeState();
};

#endif
