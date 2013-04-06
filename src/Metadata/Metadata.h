/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <string>
#include "UsableId.h"

namespace Json
{
   class Value;
};

class GameContext;
typedef class Usable Item;
class Skill;

/**
 * A global map holding all the item and skill metadata
 * (item/skill IDs and the associated names, descriptions, etc.) for the game.
 *
 * @author Noam Chitayat
 */
class Metadata
{
   /** A map of all the items, mapped by Item ID. */
   std::map<UsableId, Item*> items;

   /** A map of all the skills, mapped by Skill ID. */
   std::map<UsableId, Skill*> skills;

   Json::Value loadMetadataTable(const char* filePath);

   void loadItemMetadata();
   void loadSkillMetadata();

   public:
      /**
       * Constructor.
       * Load up all the item metadata from items.edb.
       * Load up all the skill metadata from skills.edb.
       */
      Metadata(GameContext& gameContext);

      /**
       * Destructor. Clean up the item metadata map.
       */
      ~Metadata();

      /**
       * @param key The ID of the item metadata to be retrieved.
       *
       * @return The metadata for the item with the specified ID.
       */
      Item* getItem(UsableId key) const;

      /**
       * @param key The ID of the skill metadata to be retrieved.
       *
       * @return The metadata for the skill with the specified ID.
       */
      Skill* getSkill(UsableId key) const;
};

#endif
