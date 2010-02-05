#include "PlayerCharacter.h"
#include "Sprite.h"

#include <SDL.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_TILE_ENG;

const std::string PlayerCharacter::WALKING_PREFIX = "walk_";
const std::string PlayerCharacter::STANDING_PREFIX = "stand_";

PlayerCharacter::PlayerCharacter(Spritesheet* sheet, int x, int y)
                                              : playerX(x), playerY(y),
                                                xSpeed(0), ySpeed(0)
{
   sprite = new Sprite(sheet);
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
      direction = RIGHT;
   }
   else if(keystate[SDLK_LEFT])
   {
      // Negative velocity in the x-axis
      xSpeed = -WALKING_SPEED;
      direction = LEFT;
   }
   else
   {
      xSpeed = 0;
   }

   bool moving = xSpeed != 0 || ySpeed != 0;

   if(moving)   
   {
      if(currDirection != direction)
      {
         currDirection = direction;
         switch(direction)
         {
            case LEFT:
            {
               sprite->setAnimation(WALKING_PREFIX + "left");
               break;
            }
            case RIGHT:
            {
               sprite->setAnimation(WALKING_PREFIX + "right");
               break;
            }
            case UP:
            {
               sprite->setAnimation(WALKING_PREFIX + "up");
               break;
            }
            case DOWN:
            {
               sprite->setAnimation(WALKING_PREFIX + "down");
               break;
            }
         }
      }
   }
   else
   {
      switch(direction)
      {
         case LEFT:
         {
            sprite->setFrame(STANDING_PREFIX + "left");
            break;
         }
         case RIGHT:
         {
            sprite->setFrame(STANDING_PREFIX + "right");
            break;
         }
         case UP:
         {
            sprite->setFrame(STANDING_PREFIX + "up");
            break;
         }
         case DOWN:
         {
            sprite->setFrame(STANDING_PREFIX + "down");
            break;
         }
      }
   }

   playerX += xSpeed * (timePassed / 5);
   playerY += ySpeed * (timePassed / 5);

   sprite->step(timePassed);
}

void PlayerCharacter::draw()
{
   sprite->draw(playerX, playerY);
}

PlayerCharacter::~PlayerCharacter()
{
   delete sprite;               
}
