/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "Actor.h"
#include "Listener.h"

class EntityGrid;
class PlayerData;
class CharacterRoster;
struct RosterUpdateMessage;

/**
 * The playable character in the TileEngine state. Has the same components as the NPC, but is controlled by
 * user input as opposed to a Lua script.
 *
 * The PlayerCharacter also serves as a view model that represents the player data in the Tile Engine.
 *
 * @author Noam Chitayat
 */
class PlayerCharacter : public Actor, public messaging::Listener<RosterUpdateMessage>
{
   /** The walking prefix used to load walking sprites. */
   static const std::string WALKING_PREFIX;

   /** The standing prefix used to load standing sprites. */
   static const std::string STANDING_PREFIX;
   
   /** The character roster that the character represents. */
   const CharacterRoster& roster;

   /** True iff the player entity is active on the map. */
   bool active;
  
   /** Total distance for the character to move. */
   float cumulativeDistanceCovered;

   public:
      /**
       * Constructor.
       *
       * @param messagePipe The message pipe used to send event messages.
       * @param map The map that the player character will be interacting in.
       * @param playerData The player data that the player character represents.
       * @param sheetName The name of the spritesheet to use for drawing the player character.
       */
      PlayerCharacter(messaging::MessagePipe& messagePipe, EntityGrid& map, const PlayerData& playerData, const std::string& sheetName);

      /**
       * Destructor.
       */
      ~PlayerCharacter();

      /**
       * @return true iff the player character is an active entity on the map.
       */
      bool isActive() const;

      /**
       * Adds the player entity to the map at the specified location.
       */
      void addToMap(const shapes::Point2D& location);

      /**
       * Deactivates the player entity.
       */
      void removeFromMap();

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
       * Handle an update to the character roster by keeping the
       * player actor sprite in sync with the current party leader.
       */
      void receive(const RosterUpdateMessage& message);
};

#endif
