/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "PlayerCharacter.h"
#include "Character.h"
#include "CharacterRoster.h"
#include "EntityGrid.h"
#include "MessagePipe.h"
#include "Direction.h"
#include "Pathfinder.h"
#include "PlayerData.h"
#include "RosterUpdateMessage.h"
#include "Sprite.h"

#include <math.h>
#include <SDL.h>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_TILE_ENG

const std::string PlayerCharacter::WALKING_PREFIX = "walk";
const std::string PlayerCharacter::STANDING_PREFIX = "stand";

PlayerCharacter::PlayerCharacter(messaging::MessagePipe& messagePipe, EntityGrid& map, const PlayerData& playerData) :
   Actor("player", messagePipe, map, geometry::Point2D(0, 0), geometry::Size(32, 32), 0.2f, MovementDirection::DOWN),
   m_roster(*playerData.getRoster()),
   m_active(false),
   m_cumulativeDistanceCovered(0)
{
   messagePipe.registerListener(this);
   refreshLeaderSprite();
}

bool PlayerCharacter::isActive() const
{
    return m_active;
}

void PlayerCharacter::addToMap(const geometry::Point2D& location)
{
   if(m_roster.getPartyLeader() != nullptr)
   {
      if(!m_active && m_entityGrid.addActor(this, location))
      {
         setLocation(location);
         m_active = true;
      }
   }
   else
   {
      // If there is no party leader, there is no character to show
      // so remove the player from the map until a character is set
      // as the leader.
      /** \todo Either throw an exception or return a failure signal here.
       *        We want to report this failure back to the script engine. */
      DEBUG("Unable to add player character to map; no characters in the party.");
      removeFromMap();
   }
}

void PlayerCharacter::removeFromMap()
{
   if(m_active)
   {
      flushOrders();
      m_entityGrid.removeActor(this);
      m_active = false;
   }
}

void PlayerCharacter::move(const geometry::Point2D& dst, const std::shared_ptr<Task>& task)
{
   flushOrders();
   Actor::move(dst, task);
}

void PlayerCharacter::step(long timePassed)
{
   if(!m_active) return;

   MovementDirection direction = getDirection();
   int xDirection = 0;
   int yDirection = 0;

   const Uint8 *keystate = SDL_GetKeyboardState(nullptr);
   if(!keystate[SDL_SCANCODE_UP] && keystate[SDL_SCANCODE_DOWN])
   {
      // Positive velocity in the y-axis
      direction = MovementDirection::DOWN;
      yDirection = 1;
   }
   else if(keystate[SDL_SCANCODE_UP] && !keystate[SDL_SCANCODE_DOWN])
   {
      // Negative velocity in the y-axis
      direction = MovementDirection::UP;
      yDirection = -1;
   }

   if(!keystate[SDL_SCANCODE_LEFT] && keystate[SDL_SCANCODE_RIGHT])
   {
      // Positive velocity in the x-axis
      if(direction == MovementDirection::UP)
      {
         direction = MovementDirection::UP_RIGHT;
      }
      else if(direction == MovementDirection::DOWN)
      {
         direction = MovementDirection::DOWN_RIGHT;
      }
      else
      {
         direction = MovementDirection::RIGHT;
      }

      xDirection = 1;
   }
   else if(keystate[SDL_SCANCODE_LEFT] && !keystate[SDL_SCANCODE_RIGHT])
   {
      // Negative velocity in the x-axis
      if(direction == MovementDirection::UP)
      {
         direction = MovementDirection::UP_LEFT;
      }
      else if(direction == MovementDirection::DOWN)
      {
         direction = MovementDirection::DOWN_LEFT;
      }
      else
      {
         direction = MovementDirection::LEFT;
      }

      xDirection = -1;
   }

   bool hasVelocity = xDirection != 0 || yDirection != 0;
   bool moving = hasVelocity;

   if(hasVelocity)
   {
      flushOrders();

      m_cumulativeDistanceCovered += getMovementSpeed() * timePassed;
      unsigned int distanceTraversed = 0;
      if(m_cumulativeDistanceCovered > 1.0)
      {
         distanceTraversed = floor(m_cumulativeDistanceCovered);
         m_cumulativeDistanceCovered -= distanceTraversed;
      }

      m_sprite->setAnimation(WALKING_PREFIX, direction);
      setDirection(direction);

      // In frames where distance is traversed, notify the entity grid of our movement
      // intention and set our "moving" state based on if we are actually moving
      // anywhere (e.g. haven't walked into a wall or another Actor).
      // If no actual distance is traversed, we are still moving but haven't yet had
      // time to act on the user's desired movement
      moving = distanceTraversed == 0 ||
         m_entityGrid.moveToClosestPoint(this, xDirection, yDirection, distanceTraversed);
   }
   else if(isIdle())
   {
      moving = false;
   }

   if (!moving)
   {
      m_sprite->setFrame(STANDING_PREFIX, direction);
   }

   Actor::step(timePassed);
}

void PlayerCharacter::draw() const
{
   if(m_active)
   {
      Actor::draw();
   }
}

void PlayerCharacter::refreshLeaderSprite()
{
   const Character* leader = m_roster.getPartyLeader();
   if (leader != nullptr)
   {
      setSpritesheet(leader->getSpritesheetId());
   }
   else
   {
      // If there is no party leader, there is no character to show
      // so remove the player from the map until a character is set
      // as the leader.
      DEBUG("Unable to add player character to map; no characters in the party.");
      removeFromMap();
   }
}

void PlayerCharacter::receive(const RosterUpdateMessage& message)
{
   refreshLeaderSprite();
}
