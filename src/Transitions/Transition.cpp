/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Transition.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_TRANSITIONS

Transition::Transition(Texture&& oldStateTexture, long transitionLength) :
   m_transitionLength(transitionLength),
   m_oldStateTexture(std::move(oldStateTexture))
{
   if(!m_oldStateTexture.isValid())
   {
      DEBUG("Received invalid/uninitialized screen capture for old state.");
   }
}

Transition::Transition(Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength) :
   Transition(std::move(oldStateTexture), transitionLength)
{
   m_newStateTexture = std::move(newStateTexture);
   if(!m_newStateTexture.isValid())
   {
      DEBUG("Received invalid/uninitialized screen capture for new state.");
   }
}

Transition::~Transition() = default;

bool Transition::step(long timePassed)
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
