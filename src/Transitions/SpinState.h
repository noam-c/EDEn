#ifndef SPIN_STATE_H
#define SPIN_STATE_H

#include "GameState.h"
#include "ScreenTexture.h"

class SpinState : public GameState
{
   GameState* oldState;
   GameState* newState;

   long startTime;
   long transitionLength;
   double alpha;

   long timePassed;

   bool transitionComplete;

   ScreenTexture screenTexture;

   protected:
      bool step();
      void draw();

   public:
      SpinState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength = 1000);
      ~SpinState();
};

#endif
