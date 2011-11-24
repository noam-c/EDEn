/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Animation.h"
#include "LinkedListNode.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

#define MILLISECONDS_PER_FRAME 100

Animation::Animation(FrameNode* frameIndexList) 
                                : curr(frameIndexList), timeToNextAnimation(MILLISECONDS_PER_FRAME)
{}

void Animation::update(long timePassed)
{
   timeToNextAnimation -= timePassed;
   while(timeToNextAnimation < 0)
   {
      curr = curr->next;
      timeToNextAnimation += MILLISECONDS_PER_FRAME;
   }
}

int Animation::getIndex()
{
   return curr->data;
}

Animation::~Animation()
{
}
