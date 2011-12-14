/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "Sprite.h"
#include "Spritesheet.h"
#include "Animation.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

Sprite::Sprite(Spritesheet* sheet) : sheet(sheet), frameIndex(0), animation(NULL)
{
}

void Sprite::clearCurrentFrame()
{
   if(animation)
   {
      delete animation;
      animation = NULL;
   }

   // Default to frame 0 for now.
   frameIndex = 0;
}

void Sprite::setSheet(Spritesheet* newSheet)
{
   sheet = newSheet;

   // A new sheet invalidates the current frame information.
   clearCurrentFrame();
}

void Sprite::setFrame(const std::string& frameName)
{
   int newFrameIndex = sheet->getFrameIndex(frameName);
   if(newFrameIndex < 0)
   {
      //DEBUG("Failed to find sprite frame.");
   }
   else if (animation == NULL && newFrameIndex == frameIndex)
   {
      return;
   }

   clearCurrentFrame();
   frameIndex = newFrameIndex;
}

void Sprite::setAnimation(const std::string& animationName)
{
   Animation* newAnimation = sheet->getAnimation(animationName);
   if(newAnimation == NULL)
   {
      //DEBUG("Failed to find animation.");
   }
   else if (newAnimation == animation)
   {
      return;
   }

   clearCurrentFrame();
   animation = newAnimation;
}

void Sprite::step(long timePassed)
{
   if(animation != NULL)
   {
      animation->update(timePassed);
   }
}

void Sprite::draw(int x, int y) const
{
   int indexToDraw = animation != NULL ? animation->getIndex() : frameIndex;
   sheet->draw(x, y, indexToDraw);
}

Sprite::~Sprite()
{
   clearCurrentFrame();
}
