/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <map>
#include <string>

#include "CharacterRoster.h"
#include "Inventory.h"
#include "Quest.h"

class Character;
class Item;
struct EquipSlot;

namespace Json
{
   class Value;
};

namespace messaging
{
   class MessagePipe;
};

/**
 * The exact location where the game was last saved, for reloading game state.
 *
 * @author Noam Chitayat
 */
struct SaveLocation
{
   std::string region;
   std::string map;
   int x;
   int y;
};

/**
 * A model representing the player's game data, which includes
 * character data, inventory, quest log, and game state.
 *
 * @author Noam Chitayat
 */
class PlayerData
{
   /** The file from which this player data was last saved/loaded. */
   std::string filePath;

   /** All the items that are in the player's item bag. Includes usables, keys, and unused equipment. */
   Inventory inventory;

   /** The roster of characters in the player's party or encountered by the player. */
   CharacterRoster roster;

   /** The top-level quest for the game. Contains all the quests that the player can complete. */
   Quest rootQuest;

   /** The current chapter being played (if the game was saved in the middle of a chapter. */
   std::string currChapter;

   /** The location of the last save point used. */
   SaveLocation saveLocation;
   
   void parseCharactersAndParty(Json::Value& rootElement);
   void serializeCharactersAndParty(Json::Value& outputJson) const;
    
   void parseQuestLog(Json::Value& rootElement);
   void serializeQuestLog(Json::Value& outputJson) const;
    
   void parseInventory(Json::Value& rootElement);
   void serializeInventory(Json::Value& outputJson) const;
    
   void parseLocation(Json::Value& rootElement);
   void serializeLocation(Json::Value& outputJson) const;
    
   public:
      static const int PARTY_SIZE = 4;
   
      /**
       * Constructor.
       */
      PlayerData();

      /**
       * Binds a new message pipe to send player data updates to. This overwrites the previously set
       * message pipe, if it exists.
       *
       * @param messagePipe The message pipe used to send messages about updates to the player data.
       */
      void bindMessagePipe(const messaging::MessagePipe* messagePipe);
   
      /**
       * Clears the current message pipe.
       */
      void clearMessagePipe();

      /**
       * @return The file path that this player data was last associated with (saved to or loaded from).
       */
      const std::string& getFilePath();

      /**
       * Load the player data from a file.
       *
       * @param filePath The path to load the player data from.
       */
      void load(const std::string& path);

      /**
       * Save the player data to a file and set a new default file path.
       *
       * @param filePath The path to save the player data to.
       */
      void save(const std::string& path);

      const CharacterRoster* getRoster() const;
      const Inventory* getInventory() const;
      CharacterRoster* getRoster();
      Inventory* getInventory();
   
      bool changeEquipment(Character* character, EquipSlot* slot, const Item* newEquipment);

      Quest* getRootQuest();

      ~PlayerData();
};

#endif
