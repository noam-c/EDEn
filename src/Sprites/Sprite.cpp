/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Sprite.h"

#include "Animation.h"
#include "EnumUtils.h"
#include "MovementDirection.h"
#include "ResourceLoader.h"
#include "Spritesheet.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_SPRITE

Sprite::Sprite(const std::shared_ptr<Spritesheet>& sheet) :
   m_sheet(sheet),
   m_frameIndex(0),
   m_currDirection(MovementDirection::NONE)
{
}

Sprite::~Sprite()
{
   clearCurrentFrame();
}

void Sprite::clearCurrentFrame()
{
   m_animation.reset();

   // Default to frame 0 for now.
   m_frameIndex = 0;

   m_currDirection = MovementDirection::NONE;
   m_currName = "";
}

void Sprite::setSheet(const std::shared_ptr<Spritesheet>& sheet)
{
   m_sheet = sheet;

   // A new sheet invalidates the current frame information.
   clearCurrentFrame();
}

std::string Sprite::toDirectionString(MovementDirection direction)
{
   switch(direction)
   {
      case MovementDirection::UP:
      case MovementDirection::UP_LEFT:
      case MovementDirection::UP_RIGHT:
      {
         return "_up";
      }
      case MovementDirection::DOWN:
      case MovementDirection::DOWN_LEFT:
      case MovementDirection::DOWN_RIGHT:
      {
         return "_down";
      }
      case MovementDirection::LEFT:
      {
         return "_left";
      }
      case MovementDirection::RIGHT:
      {
         return "_right";
      }
      case MovementDirection::NONE:
      default:
      {
         return "";
      }
   }
}

void Sprite::setFrame(const std::string& frameName, MovementDirection direction)
{
   if(!m_animation && frameName == m_currName && direction == m_currDirection) return;

   int frameIndex;

   if(direction == MovementDirection::NONE || (frameIndex = m_sheet->getFrameIndex(frameName + toDirectionString(direction))) < 0)
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
   if(m_animation != nullptr && animationName == m_currName && direction == m_currDirection) return;

   std::unique_ptr<Animation> animation(nullptr);

   if(direction == MovementDirection::NONE || (animation = m_sheet->getAnimation(animationName + toDirectionString(direction))) == nullptr)
   {
      animation = m_sheet->getAnimation(animationName);
   }

   if(!animation)
   {
      DEBUG("Failed to find animation.");
   }

   clearCurrentFrame();
   m_currName = animationName;
   m_currDirection = direction;
   m_animation = std::move(animation);
}

void Sprite::step(long timePassed)
{
   if(m_animation != nullptr)
   {
      m_animation->update(timePassed);
   }
}

void Sprite::draw(const shapes::Point2D& point) const
{
   int indexToDraw = m_animation != nullptr ? m_animation->getIndex() : m_frameIndex;
   m_sheet->draw(point, indexToDraw);
}
