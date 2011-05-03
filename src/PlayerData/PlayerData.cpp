#include "PlayerData.h"
#include "SaveGameItemNames.h"
#include "Character.h"
#include "Quest.h"
#include <fstream>
#include "tinyxml.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

// Uncomment this line to turn off encryption of savegames
// #define DISABLE_ENCRYPTION

void PlayerData::load(const std::string& filePath)
{
   DEBUG("Loading save file %s", filePath.c_str());

   std::ifstream input(filePath.c_str());
   if(!input)
   {
      T_T("Failed to open save game file for reading.");
   }

   TiXmlDocument xmlDoc;
   input >> xmlDoc;

   if(xmlDoc.Error())
   {
      DEBUG("Error occurred in save game XML parsing: %s", xmlDoc.ErrorDesc());
      T_T("Failed to parse save data.");
   }

   TiXmlElement* root = xmlDoc.RootElement();
   if(strcmp(root->Value(), PLAYER_DATA_ELEMENT) != 0)
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse save data.");
   }
   
   parseCharactersAndParty(root);
   parseQuestLog(root);
   parseInventory(root);
   parseLocation(root);
}

void PlayerData::parseCharactersAndParty(TiXmlElement* rootElement)
{
   TiXmlElement* charactersElement = rootElement->FirstChildElement(CHARACTER_LIST_ELEMENT);
   TiXmlElement* currCharacterNode = charactersElement->FirstChildElement(CHARACTER_ELEMENT);
   while(currCharacterNode != NULL)
   {
      Character* currCharacter = new Character(currCharacterNode);
      charactersEncountered[currCharacter->getName()] = currCharacter;

      currCharacterNode = currCharacterNode->NextSiblingElement(CHARACTER_ELEMENT);
   }
 
   TiXmlElement* partyElement = rootElement->FirstChildElement(PARTY_ELEMENT);
   currCharacterNode = partyElement->FirstChildElement(CHARACTER_ELEMENT);
   while(currCharacterNode != NULL)
   {
      std::string name = currCharacterNode->Attribute(NAME_ATTRIBUTE);
      party[name] = charactersEncountered[name];

      currCharacterNode = currCharacterNode->NextSiblingElement(CHARACTER_ELEMENT);
   }
}

void PlayerData::serializeCharactersAndParty(TiXmlElement& outputXml)
{
   TiXmlElement charactersNode(CHARACTER_LIST_ELEMENT);
   for(CharacterList::iterator iter = charactersEncountered.begin(); iter != charactersEncountered.end(); ++iter)
   {
      Character* character = iter->second;
      character->serialize(charactersNode);
   }

   TiXmlElement partyNode(PARTY_ELEMENT);
   for(CharacterList::iterator iter = party.begin(); iter != party.end(); ++iter)
   {
      Character* character = iter->second;

      TiXmlElement partyChar(CHARACTER_ELEMENT);
      partyChar.SetAttribute(NAME_ATTRIBUTE, character->getName());

      partyNode.InsertEndChild(partyChar);
   }

   outputXml.InsertEndChild(charactersNode);
   outputXml.InsertEndChild(partyNode);
}

void PlayerData::parseQuestLog(TiXmlElement* rootElement)
{
   TiXmlElement* questLog = rootElement->FirstChildElement(QUEST_ELEMENT);
   rootQuest = new Quest(questLog);
}

void PlayerData::serializeQuestLog(TiXmlElement& outputXml)
{
   rootQuest->serialize(outputXml);
}

void PlayerData::parseInventory(TiXmlElement* rootElement)
{
   TiXmlElement* itemsHeld = rootElement->FirstChildElement(INVENTORY_ELEMENT);
   TiXmlElement* currNode = itemsHeld->FirstChildElement(ITEM_ELEMENT);
   while(currNode != NULL)
   {
      int itemNum, itemQuantity;
      currNode->QueryIntAttribute(ITEM_NUM_ATTRIBUTE, &itemNum);
      currNode->QueryIntAttribute(ITEM_QUANTITY_ATTRIBUTE, &itemQuantity);

      inventory[itemNum] = itemQuantity;
      currNode = currNode->NextSiblingElement(ITEM_ELEMENT);
   } 
}

void PlayerData::serializeInventory(TiXmlElement& outputXml)
{
   TiXmlElement inventoryNode(INVENTORY_ELEMENT);
   for(ItemList::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)
   {
      int itemNumber = iter->first;
      int itemQuantity = iter->second;

      if(itemQuantity > 0)
      {
         TiXmlElement item(ITEM_ELEMENT);
         item.SetAttribute(ITEM_NUM_ATTRIBUTE, itemNumber);
         item.SetAttribute(ITEM_QUANTITY_ATTRIBUTE, itemQuantity);

         inventoryNode.InsertEndChild(item);
      }
   }

   outputXml.InsertEndChild(inventoryNode);
}

void PlayerData::parseLocation(TiXmlElement* rootElement)
{
   TiXmlElement* location = rootElement->FirstChildElement(SAVE_STATE_ELEMENT);
   if(location != NULL)
   {
      currChapter = location->Attribute(CHAPTER_ATTRIBUTE);

      // Set the current chapter and location
      SaveLocation savePoint;
      location->QueryStringAttribute(REGION_ATTRIBUTE, &savePoint.region);
      location->QueryStringAttribute(MAP_ATTRIBUTE, &savePoint.map);
      location->QueryIntAttribute(X_ATTRIBUTE, &savePoint.x);
      location->QueryIntAttribute(Y_ATTRIBUTE, &savePoint.y);
   }
   else
   {
      // Check which chapters are available for play based on the quest log
   }
}

void PlayerData::serializeLocation(TiXmlElement& outputXml)
{
}

void PlayerData::save(const std::string& filePath)
{
   TiXmlElement playerDataNode(PLAYER_DATA_ELEMENT);
   serializeCharactersAndParty(playerDataNode);
   serializeInventory(playerDataNode);
   serializeQuestLog(playerDataNode);
   serializeLocation(playerDataNode);

   DEBUG("Saving to file %s", filePath.c_str());

   TiXmlDocument xmlDoc;
   xmlDoc.InsertEndChild(playerDataNode);

#ifndef DISABLE_ENCRYPTION
   std::ofstream output(filePath.c_str());
   if(!output)
   {
      T_T("Failed to open save game file for writing.");
   }

   output << xmlDoc;
#else
   xmlDoc.SaveFile(filePath);
#endif

   if(xmlDoc.Error())
   {
      DEBUG("Error occurred in saving game XML: %s", xmlDoc.ErrorDesc());
      T_T("Failed to write save game data.");
   }
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

Character* PlayerData::getPartyLeader()
{
   return partyLeader;
}

CharacterList PlayerData::getParty()
{
   return party;
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
   delete rootQuest;

   for(CharacterList::iterator iter = charactersEncountered.begin(); iter != charactersEncountered.end(); ++iter)
   {
      delete iter->second;
   }
}