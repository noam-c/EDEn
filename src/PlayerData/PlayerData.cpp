/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "PlayerData.h"
#include "GameContext.h"
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

PlayerData::PlayerData(GameContext& gameContext) :
   gameContext(gameContext),
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

const std::string& PlayerData::getFilePath() const
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
   Json::Value& charactersElement = rootElement[PlayerData::CHARACTER_LIST_ELEMENT];
   roster.load(charactersElement);
}

void PlayerData::serializeCharactersAndParty(Json::Value& outputJson) const
{
   outputJson[PlayerData::CHARACTER_LIST_ELEMENT] = roster.serialize();
}

void PlayerData::parseQuestLog(Json::Value& rootElement)
{
   DEBUG("Loading quest log...");
   Json::Value& questLog = rootElement[Quest::QUEST_ELEMENT];
   rootQuest.load(questLog);
}

void PlayerData::serializeQuestLog(Json::Value& outputJson) const
{
   outputJson[Quest::QUEST_ELEMENT] = rootQuest.serialize();
}

void PlayerData::parseInventory(Json::Value& rootElement)
{
   DEBUG("Loading inventory data...");
   Json::Value& itemsHeld = rootElement[Inventory::INVENTORY_ELEMENT];
   inventory.load(itemsHeld);
}

void PlayerData::serializeInventory(Json::Value& outputJson) const
{
   outputJson[Inventory::INVENTORY_ELEMENT] = inventory.serialize();
}

void PlayerData::parseShortcuts(Json::Value& rootElement)
{
   // Make sure that the shortcut bar vector is properly sized and initialized.
   shortcutList.clear();
   shortcutList.resize(PlayerData::SHORTCUT_BAR_SIZE, Shortcut::getEmptyShortcut());

   DEBUG("Loading shortcuts");
   Json::Value& shortcutListJson = rootElement[PlayerData::SHORTCUTS_ELEMENT];

   for(int i = 0; i < PlayerData::SHORTCUT_BAR_SIZE && i < shortcutListJson.size(); ++i)
   {
      shortcutList[i].load(shortcutListJson[i]);
   }
}

void PlayerData::serializeShortcuts(Json::Value& outputJson) const
{
   Json::Value shortcutNode(Json::arrayValue);
   for(ShortcutList::const_iterator iter = shortcutList.begin(); iter != shortcutList.end(); ++iter)
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
      currChapter = location[PlayerData::CHAPTER_ATTRIBUTE].asString();
      
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

const Shortcut& PlayerData::getShortcut(int index) const
{
   return shortcutList[index];
}

void PlayerData::setShortcut(int index, Shortcut& shortcut)
{
   if (index < 0 || index >= PlayerData::SHORTCUT_BAR_SIZE)
   {
      DEBUG("Attempted to set shortcut %d, which is out of bounds.", index);
      return;
   }

   shortcutList[index] = Shortcut(shortcut);
}

void PlayerData::setShortcut(int index, UsableId itemId)
{
   Shortcut shortcut(itemId);
   setShortcut(index, shortcut);
}

void PlayerData::setShortcut(int index, UsableId skillId, const std::string& characterId)
{
   if(!characterId.empty() && roster.getCharacter(characterId) != NULL)
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

bool PlayerData::invokeShortcut(int index)
{
   const Shortcut& shortcut = getShortcut(index);
   if(shortcut.usableType == Shortcut::ITEM)
   {
      Item* item = gameContext.getItem(shortcut.usableId);
      if(item != NULL)
      {
         return item->use(gameContext);
      }
   }
   else if(shortcut.usableType == Shortcut::SKILL)
   {
      Skill* skill = gameContext.getSkill(shortcut.usableId);
      Character* usingCharacter = roster.getCharacter(shortcut.characterId);
      if(skill != NULL && usingCharacter != NULL)
      {
         return skill->use(gameContext, usingCharacter);
      }
   }

   return false;
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
