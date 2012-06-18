#ifndef FADE_STATE_H
#define FADE_STATE_H

#include "GameState.h"
#include "ScreenTexture.h"

class FadeState : public GameState
{
   GameState* oldState;
   GameState* newState;

   long startTime;
   long transitionLength;
   double alpha;

   bool transitionComplete;

   ScreenTexture screenTexture;

   protected:
      bool step();
      void draw();

   public:
      FadeState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength = 1000);
      ~FadeState();
};

#endif
