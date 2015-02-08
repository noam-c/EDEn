/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef PLAYER_DATA_SUMMARY_H
#define PLAYER_DATA_SUMMARY_H

#include <string>

#include "CharacterRoster.h"
#include "UsableId.h"

class Character;
class Metadata;
class PlayerData;
typedef class Usable Item;
struct EquipSlot;

namespace Json
{
   class Value;
};

/**
 * A model representing a summary of the player's game data for presentation
 * in the data menu for selecting a game to save to or load from.
 *
 * @author Noam Chitayat
 */
class PlayerDataSummary final
{
   static const char* CHARACTER_LIST_ELEMENT;
   static const char* CHARACTER_ELEMENT;

   /** The roster of characters in the player's party or encountered by the player. */
   CharacterRoster m_roster;

   void parseCharactersAndParty(Json::Value& rootElement);

   public:
      static const int PARTY_SIZE = 4;

      /**
       * Constructor.
       */
      PlayerDataSummary(const Metadata& metadata);

      /**
       * Destructor.
       */
      ~PlayerDataSummary() = default;

      /**
       * Assignment operator.
       */
      PlayerDataSummary& operator=(const PlayerData& playerData);

      /**
       * Load the player data from a file.
       *
       * @param path The path to load the player data from.
       */
      void load(const std::string& path);

      const CharacterRoster* getRoster() const;
};

#endif
