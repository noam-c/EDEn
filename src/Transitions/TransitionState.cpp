/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "TransitionState.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_TRANSITIONS

TransitionState::TransitionState(GameContext& gameContext, const std::string& stateName, Texture&& oldStateTexture, long transitionLength) :
   GameState(gameContext, GameStateType::TRANSITION, stateName),
   m_totalTime(0),
   m_transitionLength(transitionLength),
   m_progress(1.0f),
   m_oldStateTexture(std::move(oldStateTexture))
{
   if(!oldStateTexture.isValid())
   {
      DEBUG("Received invalid/uninitialized screen capture for old state.");
   }
   
   activate();
}

TransitionState::TransitionState(GameContext& gameContext, const std::string& stateName, Texture&& oldStateTexture,Texture&& newStateTexture, long transitionLength) :
   GameState(gameContext, GameStateType::TRANSITION, stateName),
   m_totalTime(0),
   m_transitionLength(transitionLength),
   m_progress(1.0f),
   m_oldStateTexture(std::move(oldStateTexture)),
   m_newStateTexture(std::move(newStateTexture))
{
   if(!oldStateTexture.isValid())
   {
      DEBUG("Received invalid/uninitialized screen capture for old state.");
   }

   if(!newStateTexture.isValid())
   {
      DEBUG("Received invalid/uninitialized screen capture for new state.");
   }

   activate();
}

TransitionState::~TransitionState() = default;

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
