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

Sprite::Sprite(Spritesheet* sheet) : sheet(sheet), frameIndex(0), animation(NULL), currDirection(NONE)
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

   currDirection = NONE;
   currName = "";
}

void Sprite::setSheet(Spritesheet* newSheet)
{
   sheet = newSheet;

   // A new sheet invalidates the current frame information.
   clearCurrentFrame();
}

std::string Sprite::toDirectionString(MovementDirection direction)
{
   switch(direction)
   {
      case UP:
      case UP_LEFT:
      case UP_RIGHT:
      {
         return "_up";
      }
      case DOWN:
      case DOWN_LEFT:
      case DOWN_RIGHT:
      {
         return "_down";
      }
      case LEFT:
      {
         return "_left";
      }
      case RIGHT:
      {
         return "_right";
      }
      case NONE:
      default:
      {
         return "";
      }
   }
}

void Sprite::setFrame(const std::string& frameName, MovementDirection direction)
{
   if(animation == NULL && frameName == currName && direction == currDirection) return;

   int newFrameIndex;
   
   if(direction == NONE || (newFrameIndex = sheet->getFrameIndex(frameName + toDirectionString(direction))) < 0)
   {
      newFrameIndex = sheet->getFrameIndex(frameName);
   }
   
   if(newFrameIndex < 0)
   {
      //DEBUG("Failed to find sprite frame.");
   }

   clearCurrentFrame();
   currName = frameName;
   currDirection = direction;
   frameIndex = newFrameIndex;
}

void Sprite::setAnimation(const std::string& animationName, MovementDirection direction)
{
   if(animation != NULL && animationName == currName && direction == currDirection) return;

   Animation* newAnimation;

   if(direction == NULL || (newAnimation = sheet->getAnimation(animationName + toDirectionString(direction))) == NULL)
   {
      newAnimation = sheet->getAnimation(animationName);
   }

   if(newAnimation == NULL)
   {
      DEBUG("Failed to find animation.");
   }

   clearCurrentFrame();
   currName = animationName;
   currDirection = direction;
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
