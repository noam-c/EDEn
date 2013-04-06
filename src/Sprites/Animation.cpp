/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Animation.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

#define MILLISECONDS_PER_FRAME 100

Animation::Animation(const std::string& name, const FrameSequence& frameSequence) :
   name(name),
   frameSequence(frameSequence),
   curr(frameSequence.begin()),
   timeToNextAnimation(MILLISECONDS_PER_FRAME)
{}

Animation::~Animation()
{
}

void Animation::update(long timePassed)
{
   timeToNextAnimation -= timePassed;
   while(timeToNextAnimation < 0)
   {
      if(++curr == frameSequence.end()) curr = frameSequence.begin();
      timeToNextAnimation += MILLISECONDS_PER_FRAME;
   }
}

int Animation::getIndex() const
{
   return *curr;
}

const std::string& Animation::getName() const
{
   return name;
}
