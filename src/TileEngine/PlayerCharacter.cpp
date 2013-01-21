/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "PlayerCharacter.h"
#include "Sprite.h"
#include "TileEngine.h"
#include "Pathfinder.h"
#include "EntityGrid.h"
#include "PlayerData.h"
#include "CharacterRoster.h"
#include "RosterUpdateMessage.h"
#include "Character.h"

#include <math.h>
#include <SDL.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const std::string PlayerCharacter::WALKING_PREFIX = "walk";
const std::string PlayerCharacter::STANDING_PREFIX = "stand";

PlayerCharacter::PlayerCharacter(messaging::MessagePipe& messagePipe, EntityGrid& map, const PlayerData& playerData, const std::string& sheetName) :
   Actor("player", sheetName, messagePipe, map, shapes::Point2D(0, 0), shapes::Size(32, 32), 1.0f, DOWN),
   roster(*playerData.getRoster()),
   active(false),
   cumulativeDistanceCovered(0)
{
   messagePipe.registerListener(this);
}

PlayerCharacter::~PlayerCharacter()
{
}

bool PlayerCharacter::isActive() const
{
    return active;
}

void PlayerCharacter::addToMap(const shapes::Point2D& location)
{
   if(roster.getPartyLeader() != NULL)
   {
      if(!active && entityGrid.addActor(this, location))
      {
         setLocation(location);
         active = true;
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
   if(active)
   {
      entityGrid.removeActor(this);
      active = false;
   }
}

void PlayerCharacter::step(long timePassed)
{
   if(!active) return;

   MovementDirection direction = getDirection();
   int xDirection = 0;
   int yDirection = 0;

   Uint8 *keystate = SDL_GetKeyState(NULL);
   if(!keystate[SDLK_UP] && keystate[SDLK_DOWN])
   {
      // Positive velocity in the y-axis
      direction = DOWN;
      yDirection = 1;      
   }
   else if(keystate[SDLK_UP] && !keystate[SDLK_DOWN])
   {
      // Negative velocity in the y-axis
      direction = UP;
      yDirection = -1;
   }

   if(!keystate[SDLK_LEFT] && keystate[SDLK_RIGHT])
   {
      // Positive velocity in the x-axis
      direction = direction == UP ? UP_RIGHT : direction == DOWN ? DOWN_RIGHT : RIGHT;
      xDirection = 1;
   }
   else if(keystate[SDLK_LEFT] && !keystate[SDLK_RIGHT])
   {
      // Negative velocity in the x-axis
      direction = direction == UP ? UP_LEFT : direction == DOWN ? DOWN_LEFT : LEFT;
      xDirection = -1;
   }

   bool hasVelocity = xDirection != 0 || yDirection != 0;
   bool moving = hasVelocity;

   if(hasVelocity)
   {
      flushOrders();
      /** \todo This algorithm is different from Actor_MoveOrder.cpp, synchronize. */
      cumulativeDistanceCovered += getMovementSpeed() * (timePassed / 5.0);
      int distanceTraversed = 0;
      if(cumulativeDistanceCovered > 1.0)
      {
         distanceTraversed = floor(cumulativeDistanceCovered);
         cumulativeDistanceCovered -= distanceTraversed;
      }

      sprite->setAnimation(WALKING_PREFIX, direction);
      setDirection(direction);
      moving = entityGrid.moveToClosestPoint(this, xDirection, yDirection, distanceTraversed);
   }
   else if(isIdle())
   {
      moving = false;
   }

   if (!moving)
   {
      sprite->setFrame(STANDING_PREFIX, direction);
   }

   Actor::step(timePassed);
}

void PlayerCharacter::draw()
{
   if(active)
   {
      Actor::draw();
   }
}

void PlayerCharacter::receive(const RosterUpdateMessage& message)
{
   const Character* leader = roster.getPartyLeader();
   if (leader != NULL)
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
