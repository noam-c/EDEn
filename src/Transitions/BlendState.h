/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef BLEND_STATE_H
#define BLEND_STATE_H

#include "TransitionState.h"

class BlendState : public TransitionState
{
   protected:
      void draw();

   public:
      BlendState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength = 1000);
      ~BlendState();
};

#endif
