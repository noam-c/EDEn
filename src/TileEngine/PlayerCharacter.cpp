/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "PlayerCharacter.h"
#include "Sprite.h"
#include "TileEngine.h"

#include <SDL.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const std::string PlayerCharacter::WALKING_PREFIX = "walk";
const std::string PlayerCharacter::STANDING_PREFIX = "stand";

PlayerCharacter::PlayerCharacter(Spritesheet* sheet, int x, int y)
                                              : playerLocation(x,y),
                                                xSpeed(0), ySpeed(0),
                                                currDirection(DOWN)
{
   sprite = new Sprite(sheet);
}

Point2D PlayerCharacter::getLocation() const
{
   return playerLocation;
}

MovementDirection PlayerCharacter::getDirection() const
{
   return currDirection;
}

void PlayerCharacter::step(long timePassed)
{
   MovementDirection direction = currDirection;

   Uint8 *keystate = SDL_GetKeyState(NULL);
   if(keystate[SDLK_UP] && keystate[SDLK_DOWN])
   {
      // Both vertical keys are pressed. Move neither up nor down.
      ySpeed = 0;
   }
   else if(keystate[SDLK_DOWN])
   {
      // Positive velocity in the y-axis
      ySpeed = WALKING_SPEED;
      direction = DOWN;
   }
   else if(keystate[SDLK_UP])
   {
      // Negative velocity in the y-axis
      ySpeed = -WALKING_SPEED;
      direction = UP;
   }
   else
   {
      ySpeed = 0;
   }

   if(keystate[SDLK_LEFT] && keystate[SDLK_RIGHT])
   {
      // Both horizontal keys are pressed. Move neither left nor right.
      xSpeed = 0;
   }
   else if(keystate[SDLK_RIGHT])
   {
      // Positive velocity in the x-axis
      xSpeed = WALKING_SPEED;
      direction = direction == UP ? UP_RIGHT : direction == DOWN ? DOWN_RIGHT : RIGHT;
   }
   else if(keystate[SDLK_LEFT])
   {
      // Negative velocity in the x-axis
      xSpeed = -WALKING_SPEED;
      direction = direction == UP ? UP_LEFT : direction == DOWN ? DOWN_LEFT : LEFT;
   }
   else
   {
      xSpeed = 0;
   }

   bool moving = xSpeed != 0 || ySpeed != 0;

   if(moving)   
   {
      sprite->setAnimation(WALKING_PREFIX, direction);
   }
   else
   {
      sprite->setFrame(STANDING_PREFIX, direction);
   }

   currDirection = direction;
   playerLocation.x += xSpeed * (timePassed / 5);
   playerLocation.y += ySpeed * (timePassed / 5);
   
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
