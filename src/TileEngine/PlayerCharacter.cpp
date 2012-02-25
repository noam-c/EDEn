/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "PlayerCharacter.h"
#include "Sprite.h"
#include "TileEngine.h"
#include "Pathfinder.h"

#include <SDL.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const std::string PlayerCharacter::WALKING_PREFIX = "walk";
const std::string PlayerCharacter::STANDING_PREFIX = "stand";

PlayerCharacter::PlayerCharacter(Pathfinder& map, const std::string& sheetName)
                                              : Actor("player", sheetName, map, 0, 0, 1.0f, DOWN), active(false)
{
}

void PlayerCharacter::addToMap(Point2D location)
{
   if(!active && pathfinder.addActor(this, location, 32, 32))
   {
      setLocation(location);
      active = true;
   }
}

void PlayerCharacter::removeFromMap()
{
   if(active)
   {
      pathfinder.removeActor(this, getLocation(), 32, 32);
      active = false;
   }
}

void PlayerCharacter::step(long timePassed)
{
   if(!active) return;

   MovementDirection direction = NONE;
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

   bool moving = xDirection != 0 || yDirection != 0;

   if(moving)
   {
      flushOrders();
      int distanceTraversed = getMovementSpeed() * (timePassed / 5);
      sprite->setAnimation(WALKING_PREFIX, direction);
      setDirection(direction);
      pathfinder.moveToClosestPoint(this, 32, 32, xDirection, yDirection, distanceTraversed);
   }
   else if(isIdle())
   {
      sprite->setFrame(STANDING_PREFIX, getDirection());
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
