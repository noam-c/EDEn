/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
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
const char* SaveLocation::DIRECTION_ATTRIBUTE = "direction";

Json::Value SaveLocation::serialize() const
{
   Json::Value locationNode(Json::nullValue);

   DEBUG("Serializing current location data...");

   // Set the current chapter and location
   locationNode[SaveLocation::REGION_ATTRIBUTE] = region;
   locationNode[SaveLocation::MAP_ATTRIBUTE] = map;
   locationNode[SaveLocation::X_ATTRIBUTE] = coords.x;
   locationNode[SaveLocation::Y_ATTRIBUTE] = coords.y;
   locationNode[SaveLocation::DIRECTION_ATTRIBUTE] = direction;

   return locationNode;
}

bool SaveLocation::isValid() const
{
   return !region.empty() && !map.empty();
}

PlayerData::PlayerData(const Metadata& metadata) :
   m_roster(metadata),
   m_rootQuest(std::string("root")),
   m_shortcutList(PlayerData::SHORTCUT_BAR_SIZE, Shortcut::getEmptyShortcut())
{
}

void PlayerData::bindMessagePipe(const messaging::MessagePipe* messagePipe)
{
   m_roster.bindMessagePipe(messagePipe);
}

void PlayerData::unbindMessagePipe()
{
   bindMessagePipe(nullptr);
}

std::tuple<std::shared_ptr<PlayerData>, SaveLocation> PlayerData::load(const std::string& path, const Metadata& metadata)
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

   auto playerData = std::make_shared<PlayerData>(metadata);

   playerData->parseCharactersAndParty(jsonRoot);
   playerData->parseQuestLog(jsonRoot);
   playerData->parseInventory(jsonRoot);
   playerData->parseShortcuts(jsonRoot);
   SaveLocation location = PlayerData::parseLocation(jsonRoot);

   return std::make_tuple(playerData, location);
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

   /** \todo Calculate which chapters are available for play based on the quest log */
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

SaveLocation PlayerData::parseLocation(Json::Value& rootElement)
{
   Json::Value& locationNode = rootElement[PlayerData::SAVE_LOCATION_ELEMENT];
   SaveLocation saveLocation;

   DEBUG("Loading current location data...");

   if(!locationNode.isNull())
   {
      // Set the current location
      saveLocation.region = locationNode[SaveLocation::REGION_ATTRIBUTE].asString();
      saveLocation.map = locationNode[SaveLocation::MAP_ATTRIBUTE].asString();

      auto x = locationNode[SaveLocation::X_ATTRIBUTE].asInt();
      auto y = locationNode[SaveLocation::Y_ATTRIBUTE].asInt();
      MovementDirection direction = static_cast<MovementDirection>(locationNode[SaveLocation::DIRECTION_ATTRIBUTE].asUInt());
      if(direction > NUM_DIRECTIONS)
      {
         direction = DOWN;
      }

      saveLocation.coords = shapes::Point2D(x,y);
      saveLocation.direction = direction;
   }

   return saveLocation;
}

void PlayerData::serializeLocation(const SaveLocation& location, Json::Value& outputJson)
{
   outputJson[PlayerData::SAVE_LOCATION_ELEMENT] = location.serialize();
}

void PlayerData::save(const SaveLocation& location, const std::string& path) const
{
   Json::Value playerDataNode(Json::objectValue);
   serializeCharactersAndParty(playerDataNode);
   serializeInventory(playerDataNode);
   serializeQuestLog(playerDataNode);
   serializeShortcuts(playerDataNode);
   PlayerData::serializeLocation(location, playerDataNode);

   DEBUG("Saving to file %s", path.c_str());

   std::ofstream output(path.c_str());
   if(!output)
   {
      T_T("Failed to open save game file for writing.");
   }

   Json::StyledStreamWriter writer("   ");
   writer.write(output, playerDataNode);
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
