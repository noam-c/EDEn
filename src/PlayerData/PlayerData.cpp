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

const char* PlayerData::SAVE_LOCATION_ELEMENT = "CurrentLocation";
const char* PlayerData::CHAPTER_ATTRIBUTE = "chapter";
const char* PlayerData::REGION_ATTRIBUTE = "region";
const char* PlayerData::MAP_ATTRIBUTE = "map";
const char* PlayerData::X_ATTRIBUTE = "x";
const char* PlayerData::Y_ATTRIBUTE = "y";

PlayerData::PlayerData(const Metadata& metadata) :
   m_roster(metadata),
   m_rootQuest(std::string("root")),
   m_shortcutList(PlayerData::SHORTCUT_BAR_SIZE, Shortcut::getEmptyShortcut())
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
      m_roster.load(playerData.m_roster.serialize());
      m_rootQuest.load(playerData.m_rootQuest.serialize());

      m_inventory = playerData.m_inventory;
      m_currChapter = playerData.m_currChapter;
      m_shortcutList = playerData.m_shortcutList;
      m_saveLocation = playerData.m_saveLocation;
   }

   return *this;
}

void PlayerData::bindMessagePipe(const messaging::MessagePipe* messagePipe)
{
   m_roster.bindMessagePipe(messagePipe);
}

void PlayerData::clearMessagePipe()
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
   for(ShortcutList::const_iterator iter = m_shortcutList.begin(); iter != m_shortcutList.end(); ++iter)
   {
      shortcutNode.append(iter->serialize());
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
   if(!location.isNull())
   {
      DEBUG("Loading current location data...");
      m_currChapter = location[PlayerData::CHAPTER_ATTRIBUTE].asString();
      
      // Set the current chapter and location
      SaveLocation savePoint;
      savePoint.region = location[PlayerData::REGION_ATTRIBUTE].asString();
      savePoint.map = location[PlayerData::MAP_ATTRIBUTE].asString();
      savePoint.x = location[PlayerData::X_ATTRIBUTE].asInt();
      savePoint.y = location[PlayerData::Y_ATTRIBUTE].asInt();
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

   Json::StyledStreamWriter writer("   ");
   writer.write(output, playerDataNode);
   
   m_filePath = path;
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
   if(!characterId.empty() && m_roster.getCharacter(characterId) != nullptr)
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
