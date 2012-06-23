/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "TransitionState.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_TRANSITIONS;

TransitionState::TransitionState(ExecutionStack& executionStack,
      GameState* oldState, GameState* newState, long transitionLength)
      : GameState(executionStack), startTime(SDL_GetTicks()), transitionLength(transitionLength), progress(1.0f)
{
   if(oldState != NULL)
   {
      oldState->activate();
      oldStateTexture.startCapture();
      oldState->drawFrame();
      oldStateTexture.endCapture();

      if (glGetError())
      {
         DEBUG("Failed to create screen capture for old state.");
      }
   }

   if(newState != NULL)
   {
      newState->activate();
      newStateTexture.startCapture();
      newState->drawFrame();
      newStateTexture.endCapture();

      if (glGetError())
      {
         DEBUG("Failed to create screen capture for new state.");
      }
   }

   activate();
}

TransitionState::~TransitionState()
{
}

bool TransitionState::step()
{
   long timePassed = SDL_GetTicks() - startTime;

   if (timePassed > transitionLength)
   {
      DEBUG("Finishing transition.");
      return false;
   }

   progress = static_cast<double>(timePassed)
         / static_cast<double>(transitionLength);
   DEBUG("Continuing transition (%dms passed).", timePassed);
   return true;
}
