/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "TransitionState.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_TRANSITIONS;

TransitionState::TransitionState(GameContext& gameContext, const std::string& stateName,
      GameState* oldState, GameState* newState, long transitionLength) :
      GameState(gameContext, GameState::TRANSITION, stateName),
      m_totalTime(0),
      m_transitionLength(transitionLength),
      m_progress(1.0f)
{
   if(oldState != NULL)
   {
      if(!captureStateToTexture(oldState, m_oldStateTexture))
      {
         DEBUG("Failed to create screen capture for old state.");
      }
   }

   if(newState != NULL)
   {
      if(!captureStateToTexture(newState, m_newStateTexture))
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
   m_totalTime += timePassed;
   if (m_totalTime > m_transitionLength)
   {
      DEBUG("Finishing transition.");
      return false;
   }

   m_progress = static_cast<double>(m_totalTime)
         / static_cast<double>(m_transitionLength);

   DEBUG("Continuing transition (%dms passed).", m_totalTime);
   return true;
}
