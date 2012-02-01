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

PlayerCharacter::PlayerCharacter(Pathfinder& map, Spritesheet* sheet, int x, int y)
                                              : map(map), playerLocation(x,y),
                                                currDirection(DOWN)
{
   sprite = new Sprite(sheet);
}

Point2D PlayerCharacter::getLocation() const
{
   return playerLocation;
}

void PlayerCharacter::setLocation(Point2D location)
{
   playerLocation = location;
}

MovementDirection PlayerCharacter::getDirection() const
{
   return currDirection;
}

void PlayerCharacter::step(long timePassed)
{
   MovementDirection direction = NONE;
   int speed = WALKING_SPEED;
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
      int distanceTraversed = speed * (timePassed / 5);
      sprite->setAnimation(WALKING_PREFIX, direction);
      currDirection = direction;
      map.moveToClosestPoint(this, 32, 32, xDirection, yDirection, distanceTraversed);
   }
   else
   {
      sprite->setFrame(STANDING_PREFIX, currDirection);
   }

   sprite->step(timePassed);
}

void PlayerCharacter::draw()
{
   sprite->draw(playerLocation.x, playerLocation.y + TileEngine::TILE_SIZE);
}

PlayerCharacter::~PlayerCharacter()
{
   delete sprite;               
}
