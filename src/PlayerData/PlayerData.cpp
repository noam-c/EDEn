/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "PlayerData.h"
#include "SaveGameItemNames.h"
#include "Character.h"
#include "ItemData.h"
#include "Item.h"
#include "Quest.h"
#include <fstream>
#include "json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

// Uncomment this line to turn off encryption of savegames
// #define DISABLE_ENCRYPTION

PlayerData::PlayerData() : partyLeader(NULL)
{
}

const std::string& PlayerData::getFilePath()
{
   return filePath;
}

void PlayerData::load(const std::string& path)
{
   DEBUG("Loading save file %s", path.c_str());

   std::ifstream input(path.c_str());
   if(!input)
   {
      T_T("Failed to open save game file for reading.");
   }

   Json::Value jsonRoot;
   input >> jsonRoot;
   
   if(jsonRoot.isNull())
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse save data.");
   }
   
   parseCharactersAndParty(jsonRoot);
   parseQuestLog(jsonRoot);
   parseInventory(jsonRoot);
   parseLocation(jsonRoot);
   
   filePath = path;
}

void PlayerData::parseCharactersAndParty(Json::Value& rootElement)
{
   Json::Value& charactersElement = rootElement[CHARACTER_LIST_ELEMENT];
   Json::Value& partyElement = charactersElement[PARTY_ELEMENT];
   Json::Value& reserveElement = charactersElement[RESERVE_ELEMENT];
   
   int partySize = partyElement.size();
   int reserveSize = reserveElement.size();
   
   if(partyElement.isArray() && partySize > 0)
   {
      DEBUG("Loading party...");
      for(int i = 0; i < partySize; ++i)
      {
         DEBUG("Adding character %d...", i+1);
         Character* currCharacter = new Character(partyElement[i]);
         std::string name = currCharacter->getName();
         charactersEncountered.push_back(currCharacter);
         party.push_back(currCharacter);
      }
      DEBUG("Party loaded.");
   }

   if(reserveElement.isArray() && reserveSize > 0)
   {
      DEBUG("Loading reserve...");
      for(int i = 0; i < reserveSize; ++i)
      {
         DEBUG("Adding character %d...", i+1);
         Character* currCharacter = new Character(reserveElement[i]);
         std::string name = currCharacter->getName();
         charactersEncountered.push_back(currCharacter);
         reserve.push_back(currCharacter);
      }
      DEBUG("Reserve loaded.");
   }
}

void PlayerData::serializeCharactersAndParty(Json::Value& outputJson)
{
   Json::Value charactersNode(Json::objectValue);
   
   Json::Value partyNode(Json::arrayValue);
   for(CharacterList::iterator iter = party.begin(); iter != party.end(); ++iter)
   {
      Character* character = *iter;
      character->serialize(partyNode);
   }
   
   Json::Value reserveNode(Json::arrayValue);
   for(CharacterList::iterator iter = reserve.begin(); iter != reserve.end(); ++iter)
   {
      Character* character = *iter;
      character->serialize(reserveNode);
   }
   
   charactersNode[PARTY_ELEMENT] = partyNode;
   charactersNode[RESERVE_ELEMENT] = reserveNode;
   
   outputJson[CHARACTER_LIST_ELEMENT] = charactersNode;
}

void PlayerData::parseQuestLog(Json::Value& rootElement)
{
   DEBUG("Loading quest log...");
   Json::Value& questLog = rootElement[QUEST_ELEMENT];
   rootQuest = new Quest(questLog);
}

void PlayerData::serializeQuestLog(Json::Value& outputJson)
{
   outputJson[QUEST_ELEMENT] = rootQuest->serialize();
}

void PlayerData::parseInventory(Json::Value& rootElement)
{
   DEBUG("Loading inventory data...");
   Json::Value& itemsHeld = rootElement[INVENTORY_ELEMENT];
   for(Json::Value::iterator iter = itemsHeld.begin(); iter != itemsHeld.end(); ++iter)
   {
      int itemNum, itemQuantity;
      itemNum = (*iter)[ITEM_NUM_ATTRIBUTE].asInt();
      itemQuantity = (*iter)[ITEM_QUANTITY_ATTRIBUTE].asInt();
      inventory.push_back(std::pair<int,int>(itemNum, itemQuantity));
   }
}

void PlayerData::serializeInventory(Json::Value& outputJson)
{
   Json::Value inventoryNode(Json::arrayValue);
   for(ItemList::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)
   {
      int itemNumber = iter->first;
      int itemQuantity = iter->second;
      
      if(itemQuantity > 0)
      {
         Json::Value itemEntry(Json::objectValue);
         itemEntry[ITEM_NUM_ATTRIBUTE] = itemNumber;
         itemEntry[ITEM_QUANTITY_ATTRIBUTE] = itemQuantity;
         
         inventoryNode.append(itemEntry);
      }
   }
   
   outputJson[INVENTORY_ELEMENT] = inventoryNode;
}

void PlayerData::parseLocation(Json::Value& rootElement)
{
   Json::Value& location = rootElement[SAVE_STATE_ELEMENT];
   if(!location.isNull())
   {
      DEBUG("Loading current location data...");
      currChapter = location[CHAPTER_ATTRIBUTE].asString();
      
      // Set the current chapter and location
      SaveLocation savePoint;
      savePoint.region = location[REGION_ATTRIBUTE].asString();
      savePoint.map = location[MAP_ATTRIBUTE].asString();
      savePoint.x = location[X_ATTRIBUTE].asInt();
      savePoint.y = location[Y_ATTRIBUTE].asInt();
   }
   else
   {
      // Check which chapters are available for play based on the quest log
   }
}

void PlayerData::serializeLocation(Json::Value& outputJson)
{
   /**
    * \todo Determine format for current location and then correctly serialize
    */
}

void PlayerData::save(const std::string& path)
{
   Json::Value playerDataNode(Json::objectValue);
   serializeCharactersAndParty(playerDataNode);
   serializeInventory(playerDataNode);
   serializeQuestLog(playerDataNode);
   serializeLocation(playerDataNode);

   DEBUG("Saving to file %s", path.c_str());

   std::ofstream output(path.c_str());
   if(!output)
   {
      T_T("Failed to open save game file for writing.");
   }

   output << playerDataNode;
   
   filePath = path;
}

void PlayerData::addNewCharacter(Character* newCharacter)
{
   std::string characterName = newCharacter->getName();
   charactersEncountered.push_back(newCharacter);
   
   if(party.empty())
   {
      partyLeader = newCharacter;
   }

   party.push_back(newCharacter);
}

Character* PlayerData::getPartyLeader() const
{
   return partyLeader;
}

Character* PlayerData::getPartyCharacter(const std::string& characterName) const
{
   for(CharacterList::const_iterator iter = party.begin(); iter != party.end(); ++iter)
   {
      if((*iter)->getName() == characterName)
      {
         return *iter;
      }
   }

   return NULL;
}

CharacterList PlayerData::getParty() const
{
   return party;
}

ItemList PlayerData::getInventory() const
{
   return inventory;
}

ItemList PlayerData::getItemsByTypes(std::vector<int> acceptedTypes) const
{
   /**
    * \todo This method needs to properly filter through the inventory by type,
    * This must be done once item type is loaded into the Item structure.
    */
   return inventory;
}

bool PlayerData::addToInventory(const Item* item, int quantity)
{
   if(quantity < 1)
   {
      return false;
   }
   
   int itemId = item->getId();
   for(ItemList::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)
   {
      if(iter->first == itemId)
      {
         iter->second += quantity;
         return true;
      }
   }
   
   inventory.push_back(std::pair<int, int>(item->getId(), quantity));
   return true;
}

bool PlayerData::removeFromInventory(const Item* item, int quantity)
{
   if(quantity < 1)
   {
      return false;
   }

   int itemId = item->getId();
   for(ItemList::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)
   {
      int& existingQuantity = iter->second;
      if(iter->first == itemId)
      {
         if(existingQuantity >= quantity)
         {
            // If the item is in the inventory in sufficient quantity, remove 'quantity' items
            // and return true.
            existingQuantity -= quantity;
            if(existingQuantity == 0)
            {
               inventory.erase(iter);
            }

            return true;
         }

         break;
      }
   }
   
   // If the item is not in inventory, return false.
   return false;
}

bool PlayerData::changeEquipment(Character* character, EquipSlot* slot, const Item* newEquipment)
{
   addToInventory(slot->equipped);
   removeFromInventory(newEquipment);
   slot->equipped = newEquipment;
   return true;
}

void PlayerData::addNewQuest(const std::string& questPath, const std::string& description, bool optionalQuest)
{
   rootQuest->addQuest(questPath, description, optionalQuest);
}

bool PlayerData::isQuestCompleted(const std::string& questPath)
{
   return rootQuest->isQuestCompleted(questPath);
}

void PlayerData::completeQuest(const std::string& questPath)
{
   rootQuest->completeQuest(questPath);
}

std::string PlayerData::getQuestDescription(const std::string& questPath)
{
   return rootQuest->getQuestDescription(questPath);
}

PlayerData::~PlayerData()
{
   for(CharacterList::iterator iter = charactersEncountered.begin(); iter != charactersEncountered.end(); ++iter)
   {
      delete *iter;
   }
   
   if(rootQuest != NULL)
   {
      delete rootQuest;
   }
}
