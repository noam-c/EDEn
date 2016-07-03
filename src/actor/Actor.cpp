/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Actor.h"
#include "ResourceLoader.h"
#include "Sprite.h"
#include "Spritesheet.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ACTOR

const std::string Actor::DEFAULT_WALKING_PREFIX = "walk";
const std::string Actor::DEFAULT_STANDING_PREFIX = "stand";

Actor::Actor(const std::string& name, const geometry::Point2D& location, const geometry::Size& size, geometry::Direction direction) :
   m_name(name),
   m_currDirection(direction),
   m_size(size),
   m_pixelLoc(location),
   m_sprite(nullptr)
{
}

Actor::Actor(Actor&&) = default;
Actor& Actor::operator=(Actor&&) = default;

Actor::~Actor() = default;

const std::string& Actor::getName() const
{
   return m_name;
}

void Actor::step(long timePassed)
{
   if(m_sprite)
   {
      m_sprite->step(timePassed);
   }
}

void Actor::draw() const
{
   if(m_sprite)
   {
      m_sprite->draw(m_pixelLoc);
   }
}

void Actor::setSpritesheet(const std::string& sheetName)
{
   std::shared_ptr<Spritesheet> sheet = ResourceLoader::getSpritesheet(sheetName);
   if(!m_sprite)
   {
      m_sprite.reset(new Sprite(sheet));
   }
   else
   {
      m_sprite->setSheet(sheet);
   }
   
   m_sprite->setFrame(Actor::DEFAULT_STANDING_PREFIX, m_currDirection);
}

void Actor::setFrame(const std::string& frameName)
{
   if(!m_sprite)
   {
      DEBUG("Failed to set sprite frame because actor %s doesn't have a sprite.", m_name.c_str());
      return;
   }
   
   m_sprite->setFrame(frameName, m_currDirection);
}

void Actor::setAnimation(const std::string& animationName)
{
   if(!m_sprite)
   {
      DEBUG("Failed to set sprite animation because actor %s doesn't have a sprite.", m_name.c_str());
      return;
   }
   
   m_sprite->setAnimation(animationName, m_currDirection);
}

void Actor::setLocation(const geometry::Point2D& location)
{
   m_pixelLoc = location;
}

const geometry::Point2D& Actor::getLocation() const
{
   return m_pixelLoc;
}

void Actor::setDirection(geometry::Direction direction)
{
   m_currDirection = direction;
}

geometry::Direction Actor::getDirection() const
{
   return m_currDirection;
}

const geometry::Size& Actor::getSize() const
{
   return m_size;
}
