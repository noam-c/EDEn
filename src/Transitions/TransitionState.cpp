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

TransitionState::TransitionState(GameContext& gameContext, const std::string& stateName,
      GameState* oldState, GameState* newState, long transitionLength) :
      GameState(gameContext, stateName),
      totalTime(0),
      transitionLength(transitionLength),
      progress(1.0f)
{
   if(oldState != NULL)
   {
      if(!captureStateToTexture(oldState, oldStateTexture))
      {
         DEBUG("Failed to create screen capture for old state.");
      }
   }

   if(newState != NULL)
   {
      if(!captureStateToTexture(newState, newStateTexture))
      {
         DEBUG("Failed to create screen capture for new state.");
      }
   }

   activate();
}

TransitionState::~TransitionState()
{
}

bool TransitionState::captureStateToTexture(GameState* state, ScreenTexture& screenTexture)
{
   state->activate();
   screenTexture.startCapture();
   state->drawFrame();
   screenTexture.endCapture();
   state->deactivate();

   return !glGetError();
}

bool TransitionState::step(long timePassed)
{
   totalTime += timePassed;
   if (totalTime > transitionLength)
   {
      DEBUG("Finishing transition.");
      return false;
   }

   progress = static_cast<double>(totalTime)
         / static_cast<double>(transitionLength);

   DEBUG("Continuing transition (%dms passed).", totalTime);
   return true;
}
