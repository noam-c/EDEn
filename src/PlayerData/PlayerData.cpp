/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "PlayerData.h"
#include "SaveGameItemNames.h"
#include "Character.h"
#include "ItemData.h"
#include "Item.h"
#include <fstream>
#include "json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

// Uncomment this line to turn off encryption of savegames
// #define DISABLE_ENCRYPTION

PlayerData::PlayerData() : partyLeader(NULL), rootQuest("root")
{
}

PlayerData::~PlayerData()
{
   for(CharacterList::iterator iter = charactersEncountered.begin(); iter != charactersEncountered.end(); ++iter)
   {
      delete *iter;
   }
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

void PlayerData::serializeCharactersAndParty(Json::Value& outputJson) const
{
   Json::Value charactersNode(Json::objectValue);
   
   Json::Value partyNode(Json::arrayValue);
   for(CharacterList::const_iterator iter = party.begin(); iter != party.end(); ++iter)
   {
      Character* character = *iter;
      character->serialize(partyNode);
   }
   
   Json::Value reserveNode(Json::arrayValue);
   for(CharacterList::const_iterator iter = reserve.begin(); iter != reserve.end(); ++iter)
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
   rootQuest.load(questLog);
}

void PlayerData::serializeQuestLog(Json::Value& outputJson) const
{
   outputJson[QUEST_ELEMENT] = rootQuest.serialize();
}

void PlayerData::parseInventory(Json::Value& rootElement)
{
   DEBUG("Loading inventory data...");
   Json::Value& itemsHeld = rootElement[INVENTORY_ELEMENT];
   inventory.load(itemsHeld);
}

void PlayerData::serializeInventory(Json::Value& outputJson) const
{
   outputJson[INVENTORY_ELEMENT] = inventory.serialize();
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

void PlayerData::serializeLocation(Json::Value& outputJson) const
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

const Inventory* PlayerData::getInventory() const
{
   return &inventory;
}

Inventory* PlayerData::getInventory()
{
   return &inventory;
}

bool PlayerData::changeEquipment(Character* character, EquipSlot* slot, const Item* newEquipment)
{
   inventory.addItem(slot->equipped->getId());
   inventory.removeItem(newEquipment->getId());
   slot->equipped = newEquipment;
   return true;
}

Quest* PlayerData::getRootQuest()
{
   return &rootQuest;
}
