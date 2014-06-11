/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "PlayerData.h"
#include "Character.h"
#include "Metadata.h"
#include "Item.h"
#include "Skill.h"
#include "Shortcut.h"
#include <fstream>
#include "json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

const char* PlayerData::CHARACTER_LIST_ELEMENT = "Characters";
const char* PlayerData::CHARACTER_ELEMENT = "Character";

const char* PlayerData::SHORTCUTS_ELEMENT = "Shortcuts";

const char* PlayerData::SAVE_LOCATION_ELEMENT = "Location";

const char* SaveLocation::REGION_ATTRIBUTE = "region";
const char* SaveLocation::MAP_ATTRIBUTE = "map";
const char* SaveLocation::X_ATTRIBUTE = "x";
const char* SaveLocation::Y_ATTRIBUTE = "y";

Json::Value SaveLocation::serialize() const
{
   Json::Value location(Json::nullValue);
   
   if(valid)
   {
      DEBUG("Serializing current location data...");
      
      // Set the current chapter and location
      location[SaveLocation::REGION_ATTRIBUTE] = region;
      location[SaveLocation::MAP_ATTRIBUTE] = map;
      location[SaveLocation::X_ATTRIBUTE] = coords.x;
      location[SaveLocation::Y_ATTRIBUTE] = coords.y;
   }
   
   return location;
}

PlayerData::PlayerData(const Metadata& metadata) :
   m_roster(metadata),
   m_rootQuest(std::string("root")),
   m_shortcutList(PlayerData::SHORTCUT_BAR_SIZE, Shortcut::getEmptyShortcut())
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
      m_roster.load(playerData.m_roster.serialize());
      m_rootQuest.load(playerData.m_rootQuest.serialize());

      m_inventory = playerData.m_inventory;
      m_shortcutList = playerData.m_shortcutList;
      m_saveLocation = playerData.m_saveLocation;
   }

   return *this;
}

void PlayerData::bindMessagePipe(const messaging::MessagePipe* messagePipe)
{
   m_roster.bindMessagePipe(messagePipe);
}

void PlayerData::unbindMessagePipe()
{
   bindMessagePipe(nullptr);
}

const std::string& PlayerData::getFilePath() const
{
   return m_filePath;
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

   m_filePath = path;
}

void PlayerData::parseCharactersAndParty(Json::Value& rootElement)
{
   DEBUG("Loading character roster...");
   Json::Value& charactersElement = rootElement[PlayerData::CHARACTER_LIST_ELEMENT];
   m_roster.load(charactersElement);
}

void PlayerData::serializeCharactersAndParty(Json::Value& outputJson) const
{
   outputJson[PlayerData::CHARACTER_LIST_ELEMENT] = m_roster.serialize();
}

void PlayerData::parseQuestLog(Json::Value& rootElement)
{
   DEBUG("Loading quest log...");
   Json::Value& questLog = rootElement[Quest::QUEST_ELEMENT];
   m_rootQuest.load(questLog);
}

void PlayerData::serializeQuestLog(Json::Value& outputJson) const
{
   outputJson[Quest::QUEST_ELEMENT] = m_rootQuest.serialize();
}

void PlayerData::parseInventory(Json::Value& rootElement)
{
   DEBUG("Loading inventory data...");
   Json::Value& itemsHeld = rootElement[Inventory::INVENTORY_ELEMENT];
   m_inventory.load(itemsHeld);
}

void PlayerData::serializeInventory(Json::Value& outputJson) const
{
   outputJson[Inventory::INVENTORY_ELEMENT] = m_inventory.serialize();
}

void PlayerData::parseShortcuts(Json::Value& rootElement)
{
   // Make sure that the shortcut bar vector is properly sized and initialized.
   m_shortcutList.clear();
   m_shortcutList.resize(PlayerData::SHORTCUT_BAR_SIZE, Shortcut::getEmptyShortcut());

   DEBUG("Loading shortcuts");
   Json::Value& shortcutListJson = rootElement[PlayerData::SHORTCUTS_ELEMENT];

   for(int i = 0; i < PlayerData::SHORTCUT_BAR_SIZE && i < shortcutListJson.size(); ++i)
   {
      m_shortcutList[i].load(shortcutListJson[i]);
   }
}

void PlayerData::serializeShortcuts(Json::Value& outputJson) const
{
   Json::Value shortcutNode(Json::arrayValue);
   for(const auto& shortcut : m_shortcutList)
   {
      shortcutNode.append(shortcut.serialize());
   }

   int remainingShortcutSlotCount = PlayerData::SHORTCUT_BAR_SIZE - shortcutNode.size();
   if(remainingShortcutSlotCount > 0)
   {
      const Json::Value emptyShortcutJson = Shortcut::getEmptyShortcut().serialize();
      for(int i = 0; i < remainingShortcutSlotCount; ++i)
      {
         shortcutNode.append(emptyShortcutJson);
      }
   }

   outputJson[PlayerData::SHORTCUTS_ELEMENT] = shortcutNode;
}

void PlayerData::parseLocation(Json::Value& rootElement)
{
   Json::Value& location = rootElement[PlayerData::SAVE_LOCATION_ELEMENT];
   m_saveLocation.valid = false;

   DEBUG("Loading current location data...");

   if(!location.isNull())
   {
      m_saveLocation.valid = true;

      // Set the current location
      m_saveLocation.region = location[SaveLocation::REGION_ATTRIBUTE].asString();
      m_saveLocation.map = location[SaveLocation::MAP_ATTRIBUTE].asString();

      auto x = location[SaveLocation::X_ATTRIBUTE].asInt();
      auto y = location[SaveLocation::Y_ATTRIBUTE].asInt();

      m_saveLocation.coords = shapes::Point2D(x,y);
   }

   /** \todo Calculate which chapters are available for play based on the quest log */
}

void PlayerData::serializeLocation(Json::Value& outputJson) const
{
   outputJson[PlayerData::SAVE_LOCATION_ELEMENT] = m_saveLocation.serialize();
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

   Json::StyledStreamWriter writer("   ");
   writer.write(output, playerDataNode);

   m_filePath = path;
}

const SaveLocation& PlayerData::getSaveLocation() const
{
   return m_saveLocation;
}

const CharacterRoster* PlayerData::getRoster() const
{
   return &m_roster;
}

const Inventory* PlayerData::getInventory() const
{
   return &m_inventory;
}

const Shortcut& PlayerData::getShortcut(int index) const
{
   return m_shortcutList[index];
}

void PlayerData::setShortcut(int index, Shortcut& shortcut)
{
   if (index < 0 || index >= PlayerData::SHORTCUT_BAR_SIZE)
   {
      DEBUG("Attempted to set shortcut %d, which is out of bounds.", index);
      return;
   }

   m_shortcutList[index] = Shortcut(shortcut);
}

void PlayerData::setShortcut(int index, UsableId itemId)
{
   Shortcut shortcut(itemId);
   setShortcut(index, shortcut);
}

void PlayerData::setShortcut(int index, UsableId skillId, const std::string& characterId)
{
   if(!characterId.empty() && m_roster.characterExists(characterId))
   {
      Shortcut shortcut(skillId, characterId);
      setShortcut(index, shortcut);
   }
}

void PlayerData::clearShortcut(int index)
{
   Shortcut emptyShortcut = Shortcut::getEmptyShortcut();
   setShortcut(index, emptyShortcut);
}

CharacterRoster* PlayerData::getRoster()
{
   return &m_roster;
}

Inventory* PlayerData::getInventory()
{
   return &m_inventory;
}

bool PlayerData::changeEquipment(Character* character, EquipSlot* slot, const Item* newEquipment)
{
   m_inventory.addItem(slot->equipped->getId());
   m_inventory.removeItem(newEquipment->getId());
   slot->equipped = newEquipment;
   return true;
}

Quest* PlayerData::getRootQuest()
{
   return &m_rootQuest;
}
