/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <map>
#include <string>
#include <vector>

#include "ItemList.h"

class Character;
class Item;
class Quest;
struct EquipSlot;

namespace Json
{
   class Value;
};

typedef std::vector<Character*> CharacterList;

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
   
   /**
    * A list of all playable characters encountered.
    * This includes characters in the party and the reserve,
    * but also includes characters encountered by the player,
    * but currently inaccessible for play.
    */
   CharacterList charactersEncountered;

   /** The characters in the main party, who interact with the world and participate in combat. */
   CharacterList party;
   
   /** The characters who are available to switch into the party. */
   CharacterList reserve;

   /** The lead character in the party, who the player sees when they are playing the game. */
   Character* partyLeader;

   /** All the items that are in the player's item bag. Includes usables, keys, and unused equipment. */
   ItemList inventory;

   /** The top-level quest for the game. Contains all the quests that the player can complete. */
   Quest* rootQuest;

   /** The current chapter being played (if the game was saved in the middle of a chapter. */
   std::string currChapter;

   /** The location of the last save point used. */
   SaveLocation saveLocation;
   
   void parseCharactersAndParty(Json::Value& rootElement);
   void serializeCharactersAndParty(Json::Value& outputJson);
    
   void parseQuestLog(Json::Value& rootElement);
   void serializeQuestLog(Json::Value& outputJson);
    
   void parseInventory(Json::Value& rootElement);
   void serializeInventory(Json::Value& outputJson);
    
   void parseLocation(Json::Value& rootElement);
   void serializeLocation(Json::Value& outputJson);
    
   public:
      static const int PARTY_SIZE = 4;
   
      /**
       * Constructor.
       */
      PlayerData();
   
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
       * Save the player data to a file.
       *
       * @param filePath The path to save the player data to.
       */
      void save(const std::string& path);
   
      void addNewCharacter(Character* newCharacter);
      Character* getPartyLeader() const;
      Character* getPartyCharacter(const std::string& characterName) const;

	   CharacterList getParty() const;
	   ItemList getInventory() const;
      ItemList getItemsByTypes(std::vector<int> acceptedTypes) const;
   
      bool addToInventory(const Item* item, int quantity = 1);
      bool removeFromInventory(const Item* item, int quantity = 1);
   
      bool changeEquipment(Character* character, EquipSlot* slot, const Item* newEquipment);

      void addNewQuest(const std::string& questPath, const std::string& description, bool optionalQuest);
      bool isQuestCompleted(const std::string& questPath);
      void completeQuest(const std::string& questPath);
      std::string getQuestDescription(const std::string& questPath);

      ~PlayerData();
};

#endif
