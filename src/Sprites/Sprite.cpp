/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Sprite.h"
#include "Spritesheet.h"
#include "Animation.h"
#include "ResourceLoader.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_SPRITE;

Sprite::Sprite(Spritesheet* sheet) :
   m_sheet(sheet),
   m_frameIndex(0),
   m_animation(NULL),
   m_currDirection(NONE)
{
}

Sprite::~Sprite()
{
   clearCurrentFrame();
}

void Sprite::clearCurrentFrame()
{
   if(m_animation != NULL)
   {
      delete m_animation;
      m_animation = NULL;
   }

   // Default to frame 0 for now.
   m_frameIndex = 0;

   m_currDirection = NONE;
   m_currName = "";
}

void Sprite::setSheet(Spritesheet* sheet)
{
   m_sheet = sheet;

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
   if(m_animation == NULL && frameName == m_currName && direction == m_currDirection) return;

   int frameIndex;
   
   if(direction == NONE || (frameIndex = m_sheet->getFrameIndex(frameName + toDirectionString(direction))) < 0)
   {
      frameIndex = m_sheet->getFrameIndex(frameName);
   }
   
   if(frameIndex < 0)
   {
      //DEBUG("Failed to find sprite frame.");
   }

   clearCurrentFrame();
   m_currName = frameName;
   m_currDirection = direction;
   m_frameIndex = frameIndex;
}

void Sprite::setAnimation(const std::string& animationName, MovementDirection direction)
{
   if(m_animation != NULL && animationName == m_currName && direction == m_currDirection) return;

   Animation* animation;

   if(direction == NONE || (animation = m_sheet->getAnimation(animationName + toDirectionString(direction))) == NULL)
   {
      animation = m_sheet->getAnimation(animationName);
   }

   if(animation == NULL)
   {
      DEBUG("Failed to find animation.");
   }

   clearCurrentFrame();
   m_currName = animationName;
   m_currDirection = direction;
   m_animation = animation;
}

void Sprite::step(long timePassed)
{
   if(m_animation != NULL)
   {
      m_animation->update(timePassed);
   }
}

void Sprite::draw(const shapes::Point2D& point) const
{
   int indexToDraw = m_animation != NULL ? m_animation->getIndex() : m_frameIndex;
   m_sheet->draw(point, indexToDraw);
}
