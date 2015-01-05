/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Animation.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

#define MILLISECONDS_PER_FRAME 100

Animation::Animation(const std::string& name, const FrameSequence& frameSequence) :
   m_name(name),
   m_frameSequence(frameSequence),
   m_curr(frameSequence.begin()),
   m_timeToNextAnimation(MILLISECONDS_PER_FRAME)
{}

void Animation::update(long timePassed)
{
   m_timeToNextAnimation -= timePassed;
   while(m_timeToNextAnimation < 0)
   {
      if(++m_curr == m_frameSequence.end()) m_curr = m_frameSequence.begin();
      m_timeToNextAnimation += MILLISECONDS_PER_FRAME;
   }
}

int Animation::getIndex() const
{
   return *m_curr;
}

const std::string& Animation::getName() const
{
   return m_name;
}
