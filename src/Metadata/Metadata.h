/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <string>
#include "GameStateType.h"
#include "UsableId.h"
#include "Item.h"
#include "Skill.h"

namespace Json
{
   class Value;
};

class Character;
class GameContext;

typedef class Usable Item;
class Skill;
class ScriptEngine;

/**
 * A global map holding all the item and skill metadata
 * (item/skill IDs and the associated names, descriptions, etc.) for the game.
 *
 * @author Noam Chitayat
 */
class Metadata
{
   /** The script engine used to invoke item and skill scripts. */
   ScriptEngine& m_scriptEngine;

   /** A map of all the items, mapped by Item ID. */
   std::map<UsableId, Item> m_items;

   /** A map of all the skills, mapped by Skill ID. */
   std::map<UsableId, Skill> m_skills;

   Json::Value loadMetadataTable(const char* filePath);

   void loadItemMetadata();
   void loadSkillMetadata();

   public:
      /**
       * Constructor.
       * Load up all the item metadata from items.edb.
       * Load up all the skill metadata from skills.edb.
       *
       * @param scriptEngine The script engine to use when invoking items and skills.
       */
      Metadata(ScriptEngine& scriptEngine);

      /**
       * @param key The ID of the item metadata to be retrieved.
       *
       * @return The metadata for the item with the specified ID.
       */
      const Item* getItem(UsableId key) const;

      /**
       * @param key The ID of the skill metadata to be retrieved.
       *
       * @return The metadata for the skill with the specified ID.
       */
      const Skill* getSkill(UsableId key) const;

      bool useItem(UsableId key, const GameStateType stateType);
      bool useSkill(UsableId key, const GameStateType stateType, Character* usingCharacter);
};

#endif
