#include "PlayerData.h"
#include "SaveGameItemNames.h"
#include "Character.h"
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

void PlayerData::load(const std::string& filePath)
{
   DEBUG("Loading save file %s", filePath.c_str());

   std::ifstream input(filePath.c_str());
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
         charactersEncountered[name] = currCharacter;
         party[name] = currCharacter;
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
         charactersEncountered[name] = currCharacter;
         reserveElement[name] = currCharacter;
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
      Character* character = iter->second;
      character->serialize(partyNode);
   }
   
   Json::Value reserveNode(Json::arrayValue);
   for(CharacterList::iterator iter = reserve.begin(); iter != reserve.end(); ++iter)
   {
      Character* character = iter->second;
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
      inventory[itemNum] = itemQuantity;
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

void PlayerData::save(const std::string& filePath)
{
   Json::Value playerDataNode(Json::objectValue);
   serializeCharactersAndParty(playerDataNode);
   serializeInventory(playerDataNode);
   serializeQuestLog(playerDataNode);
   serializeLocation(playerDataNode);

   DEBUG("Saving to file %s", filePath.c_str());

   std::ofstream output(filePath.c_str());
   if(!output)
   {
      T_T("Failed to open save game file for writing.");
   }

   output << playerDataNode;
}

void PlayerData::addNewCharacter(Character* newCharacter)
{
   std::string characterName = newCharacter->getName();
   charactersEncountered[characterName] = newCharacter;
   
   if(party.empty())
   {
      partyLeader = newCharacter;
   }

   party[characterName] = newCharacter;
}

Character* PlayerData::getPartyLeader() const
{
   return partyLeader;
}

Character* PlayerData::getPartyCharacter(const std::string& characterName) const
{
   return party.at(characterName);
}

CharacterList PlayerData::getParty() const
{
   return party;
}

ItemList PlayerData::getInventory() const
{
   return inventory;
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
      delete iter->second;
   }
   
   if(rootQuest != NULL)
   {
      delete rootQuest;
   }
}
