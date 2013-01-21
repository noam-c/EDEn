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

PlayerData::PlayerData(const GameContext& gameContext) :
   roster(gameContext),
   rootQuest(std::string("root")),
   shortcutList(PlayerData::SHORTCUT_BAR_SIZE, 0)
{
}

PlayerData::~PlayerData()
{
}

PlayerData& PlayerData::operator=(const PlayerData& playerData)
{
   if(&playerData != this)
   {
      /**
       * \todo Once the project moves to C++11, we can use STL smart pointers
       * to manage the members of these objects. After that, these serializations and reloads
       * can be replaced with proper assignment operators without bloating the code to
       * copy objects on the heap.
       */
      roster.load(playerData.roster.serialize());
      rootQuest.load(playerData.rootQuest.serialize());

      inventory = playerData.inventory;
      currChapter = playerData.currChapter;
      shortcutList = playerData.shortcutList;
      saveLocation = playerData.saveLocation;
   }

   return *this;
}

void PlayerData::bindMessagePipe(const messaging::MessagePipe* messagePipe)
{
   roster.bindMessagePipe(messagePipe);
}

void PlayerData::clearMessagePipe()
{
   bindMessagePipe(NULL);
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
   parseShortcuts(jsonRoot);
   parseLocation(jsonRoot);
   
   filePath = path;
}

void PlayerData::parseCharactersAndParty(Json::Value& rootElement)
{
   DEBUG("Loading character roster...");
   Json::Value& charactersElement = rootElement[CHARACTER_LIST_ELEMENT];
   roster.load(charactersElement);
}

void PlayerData::serializeCharactersAndParty(Json::Value& outputJson) const
{
   outputJson[CHARACTER_LIST_ELEMENT] = roster.serialize();
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

void PlayerData::parseShortcuts(Json::Value& rootElement)
{
   // Make sure that the shortcut bar vector is properly sized and initialized.
   shortcutList.clear();
   shortcutList.resize(PlayerData::SHORTCUT_BAR_SIZE, 0);

   DEBUG("Loading shortcuts");
   Json::Value& shortcutListJson = rootElement[SHORTCUTS_ELEMENT];

   for(int i = 0; i < PlayerData::SHORTCUT_BAR_SIZE && i < shortcutListJson.size(); ++i)
   {
      int id = shortcutListJson[i].asInt();
      DEBUG("Loading shortcut with item ID: %d", id);
      shortcutList[i] = id;
   }
}

void PlayerData::serializeShortcuts(Json::Value& outputJson) const
{
   Json::Value shortcutNode(Json::arrayValue);
   for(ShortcutList::const_iterator iter = shortcutList.begin(); iter != shortcutList.end(); ++iter)
   {
      int itemNumber = *iter;
      shortcutNode.append(itemNumber);
   }

   int remainingShortcutSlotCount = PlayerData::SHORTCUT_BAR_SIZE - shortcutNode.size();
   for(int i = 0; i < remainingShortcutSlotCount; ++i)
   {
      shortcutNode.append(0);
   }

   outputJson[SHORTCUTS_ELEMENT] = shortcutNode;
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
   serializeShortcuts(playerDataNode);
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

const CharacterRoster* PlayerData::getRoster() const
{
   return &roster;
}

const Inventory* PlayerData::getInventory() const
{
   return &inventory;
}

int PlayerData::getShortcut(int index) const
{
   return shortcutList[index];
}

void PlayerData::setShortcut(int index, int itemId)
{
   if (index < 0 || index >= PlayerData::SHORTCUT_BAR_SIZE)
   {
      DEBUG("Attempted to set shortcut %d, which is out of bounds.", index);
      return;
   }

   shortcutList[index] = itemId;
}

void PlayerData::clearShortcut(int index)
{
   setShortcut(index, 0);
}

CharacterRoster* PlayerData::getRoster()
{
   return &roster;
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
