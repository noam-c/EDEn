/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <map>
#include <string>

#include "CharacterRoster.h"
#include "Inventory.h"
#include "Point2D.h"
#include "Quest.h"
#include "Shortcut.h"
#include "UsableId.h"

class Character;
typedef class Usable Item;
struct EquipSlot;
class Metadata;

namespace geometry
{
   enum class Direction;
};

namespace Json
{
   class Value;
};

namespace messaging
{
   class MessagePipe;
};

/**
 * The exact location where the game was last saved, for reloading game state.
 *
 * @author Noam Chitayat
 */
struct SaveLocation
{
   static const char* REGION_ATTRIBUTE;
   static const char* MAP_ATTRIBUTE;
   static const char* X_ATTRIBUTE;
   static const char* Y_ATTRIBUTE;
   static const char* DIRECTION_ATTRIBUTE;

   std::string region;
   std::string map;
   geometry::Point2D coords;
   geometry::Direction direction;

   Json::Value serialize() const;
   bool isValid() const;
};

struct Shortcut;

/**
 * A model representing the player's game data, which includes
 * character data, inventory, quest log, and game state.
 *
 * @author Noam Chitayat
 */
class PlayerData
{
   static const char* CHARACTER_LIST_ELEMENT;
   static const char* CHARACTER_ELEMENT;

   static const char* SAVE_LOCATION_ELEMENT;

   static const char* SHORTCUTS_ELEMENT;

   /** All the items that are in the player's item bag. Includes usables, keys, and unused equipment. */
   Inventory m_inventory;

   /** The roster of characters in the player's party or encountered by the player. */
   CharacterRoster m_roster;

   /** The top-level quest for the game. Contains all the quests that the player can complete. */
   Quest m_rootQuest;

   /** The player's shortcut list. */
   std::vector<Shortcut> m_shortcutList;

   void parseCharactersAndParty(Json::Value& rootElement);
   void serializeCharactersAndParty(Json::Value& outputJson) const;

   void parseQuestLog(Json::Value& rootElement);
   void serializeQuestLog(Json::Value& outputJson) const;

   void parseInventory(Json::Value& rootElement);
   void serializeInventory(Json::Value& outputJson) const;

   void parseShortcuts(Json::Value& rootElement);
   void serializeShortcuts(Json::Value& outputJson) const;

   static SaveLocation parseLocation(Json::Value& rootElement);
   static void serializeLocation(const SaveLocation& location, Json::Value& outputJson);

   void setShortcut(int index, Shortcut& shortcut);

   public:
      static const int PARTY_SIZE = 4;
      static const int SHORTCUT_BAR_SIZE = 10;

      /**
       * Constructor.
       */
      PlayerData(const Metadata& metadata);

      /**
       * Binds a new message pipe to send player data updates to. This overwrites the previously set
       * message pipe, if it exists.
       *
       * @param messagePipe The message pipe used to send messages about updates to the player data.
       */
      void bindMessagePipe(const messaging::MessagePipe* messagePipe);

      /**
       * Unbinds the current message pipe.
       */
      void unbindMessagePipe();

      /**
       * Load the player data from a file.
       *
       * @param path The path to load the player data from.
       */
      static std::tuple<std::shared_ptr<PlayerData>, SaveLocation> load(const std::string& path, const Metadata& metadata);

      /**
       * Save the player data to a file and set a new default file path.
       *
       * @param path The path to save the player data to.
       */
      void save(const SaveLocation& location, const std::string& path) const;

      const Shortcut& getShortcut(int index) const;
      void setShortcut(int index, UsableId itemId);
      void setShortcut(int index, UsableId itemId, const std::string& characterId);
      void clearShortcut(int index);

      const CharacterRoster* getRoster() const;
      const Inventory* getInventory() const;
      CharacterRoster* getRoster();
      Inventory* getInventory();

      bool changeEquipment(Character* character, EquipSlot* slot, const Item* newEquipment);

      Quest* getRootQuest();
};

#endif
