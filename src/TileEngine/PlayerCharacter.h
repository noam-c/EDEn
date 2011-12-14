/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <string>

#include "MovementDirection.h"
#include "Point2D.h"

class Spritesheet;
class Sprite;

/**
 * The playable character in the TileEngine state. Has the same components as the NPC, but is controlled by
 * user input as opposed to a Lua script.
 *
 * @author Noam Chitayat
 */
class PlayerCharacter
{
   /** The speed of movement for the player character when walking (as opposed to running. */
   static const int WALKING_SPEED = 1;

   /** The walking prefix used to load walking sprites. */
   static const std::string WALKING_PREFIX;

   /** The standing prefix used to load standing sprites. */
   static const std::string STANDING_PREFIX;
   
   /** The player's location on the map. */
   Point2D playerLocation;
   
   /** The player's horizontal and vertical speed. */
   int xSpeed, ySpeed;
   
   /** The current direction that the character is facing. */
   MovementDirection currDirection;

   /** The sprite to draw to represent the character. */
   Sprite* sprite;

   public:
   
      /**
       * Constructor.
       *
       * @param sheet The spritesheet to use for drawing the player character.
       * @param x The x-location of the player character.
       * @param y The y-location of the player character.
       */
      PlayerCharacter(Spritesheet* sheet, int x, int y);
   
      /**
       * Takes player input and determines the character's direction and speed,
       * as well as updating the location based on the speed.
       */
      void step(long timePassed);
   
      /**
       * Draws the player character at the playerLocation coordinates.
       */
      void draw();
   
      /**
       * Destructor.
       */
      ~PlayerCharacter();
};

#endif
