/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include <string>

#include "MovementDirection.h"
#include "Point2D.h"

class Pathfinder;
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
   /** The speed of movement for the player character when walking (as opposed to running). */
   static const int WALKING_SPEED = 1;

   /** The walking prefix used to load walking sprites. */
   static const std::string WALKING_PREFIX;

   /** The standing prefix used to load standing sprites. */
   static const std::string STANDING_PREFIX;

   /** The map that the player character is standing in. */
   Pathfinder& map;
   
   /** True iff the player entity is active on the map. */
   bool active;
   
   /** The player's location on the map. */
   Point2D playerLocation;
   
   /** The current direction that the character is facing. */
   MovementDirection currDirection;

   /** The sprite to draw to represent the character. */
   Sprite* sprite;

   public:
   
      /**
       * Constructor.
       *
       * @param map The map that the player character will be sitting in.
       * @param sheet The spritesheet to use for drawing the player character.
       */
      PlayerCharacter(Pathfinder& map, Spritesheet* sheet);
      
      /**
       * @return The location of the player character.
       */
      Point2D getLocation() const;

      /**
       * Sets a new location for the player.
       *
       * @param location The new location to set.
       */
      void setLocation(Point2D location);

      /**
       * Adds the player entity to the map at the specified location.
       */
      void addToMap(Point2D location);

      /**
       * Deactivates the player entity.
       */
      void removeFromMap();

      /**
       * @return The direction that the player character is facing.
       */
      MovementDirection getDirection() const;

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
