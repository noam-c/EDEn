/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <map>
#include <memory>
#include "EquipData.h"
#include "SkillList.h"

namespace Json
{
   class Value;
};

class Aspect;
class CharacterArchetype;
class Metadata;

/**
 * A model holding the data for a character that can be used by the player.
 * All relevant attributes, skills and equipment should be held in this class.
 *
 * @author Noam Chitayat
 */
class Character final
{
   static const char* ARCHETYPE_ATTRIBUTE;
   static const char* NAME_ATTRIBUTE;
   static const char* SCHEMA_ATTRIBUTE;

   static const char* BASE_STATS_ELEMENT;
   static const char* CURRENT_STATS_ELEMENT;
   static const char* SKILLS_ELEMENT;
   static const char* EQUIPMENT_ELEMENT;
   static const char* ASPECTS_ELEMENT;

   static const char* SPRITESHEET_ATTRIBUTE;
   static const char* PORTRAIT_ELEMENT;
   static const char* PORTRAIT_PATH_ATTRIBUTE;

   static const char* LEVEL_ATTRIBUTE;
   static const char* ASPECT_ATTRIBUTE;

   static const char* MAX_HP_ATTRIBUTE;
   static const char* MAX_SP_ATTRIBUTE;
   static const char* HP_ATTRIBUTE;
   static const char* SP_ATTRIBUTE;

   /** Represents a set of skills and the amount of times the Character has successfully used them. */
   typedef std::map<UsableId, unsigned int> SkillUsage;

   /** The metadata containing skill information. */
   Metadata& m_metadata;

   bool m_initialized = false;
   
   /** The character's ID for data lookups. */
   std::string m_id;

   /** The name of the character. */
   std::string m_name;

   /** The schema used to create the character. */
   std::string m_schema;
   
   /** The archetype that was loaded for this character (empty if the default archetype was used). */
   std::string m_archetype;

   /** The currently available Aspects for this character. */
   std::vector<std::unique_ptr<Aspect>> m_archetypeAspects;

   /** The currently chosen Aspect for this character. */
   unsigned int m_selectedAspect = 0;

   /** The path to the character portrait (used in dialogue and menus). */
   std::string m_portraitPath;

   /** The ID of the spritesheet resource used for this character. */
   std::string m_spritesheetId;

   /** Character base stats, from which current stats are derived. */
   std::map<std::string, int> m_baseStats;

   /** Current character stats */
   std::map<std::string, int> m_currentStats;
   
   /** Current level of the character. */
   unsigned int m_level;

   /** The equipment worn by this Character. */
   EquipData m_equipment;

   /** The skills known by this Character. */
   SkillList m_availableSkills;

   /** The skills that the Character has used. */
   SkillUsage m_skillUsage;

   /**
    * Loads the archetype for the character, which provides character
    * information and base stats/equipment.
    *
    * @param archetypeId The ID of the archetype to load.
    *
    * @return A JSON node containing the loaded character archetype.
    */
   static Json::Value loadArchetype(const std::string& archetypeId);

   /**
    * Parse archetype data from either a saved character or an archetype.
    *
    * @param archetypeData The archetype data to load.
    */
   void parseArchetypeData(const Metadata& metadata, const Json::Value& archetypeData);

   /**
    * Parse the portrait path from the character node.
    *
    * @param portraitDataContainer The JSON node containing the portrait data to load.
    */
   void parsePortraitData(const Json::Value& portraitDataContainer);

   /**
    * Parse the aspect data from the aspect array.
    *
    * @param aspectsDataContainer The JSON node containing the list of aspects to load.
    */
   void parseAspects(const Json::Value& aspectsDataContainer);

   /**
    * Parse the base stats from the given node.
    *
    * @param baseStatsDataContainer The JSON node containing the base stats to load.
    */
   void parseBaseStats(const Json::Value& baseStatsDataContainer);

   /**
    * Parse the current stats from the given node.
    *
    * @param statsDataContainer The JSON node containing the stats to load.
    */
   void parseCurrentStats(const Json::Value& statsDataContainer);

   /**
    * Parse the skill list from the given node.
    *
    * @param skillsDataContainer The JSON node containing the stats to load.
    */
   void parseSkills(const Json::Value& skillsDataContainer);

   void refreshAvailableSkills();

   /**
    * Constructor used to create a new character.
    * Initializes the entire character using the base archetype data.
    *
    * @param id The id of the new character.
    * @param level The starting level of the new character.
    */
   Character(Metadata& metadata, const std::string& id, int level = 1);

   /**
    * Constructor used to load an existing character.
    *
    * @param charToLoad The JSON node containing the character's data.
    */
   Character(Metadata& metadata, const std::string& id, const Json::Value& charToLoad);

   public:
      static Character createCharacter(Metadata& metadata, const std::string& id, int level = 1);

      static Character loadCharacter(Metadata& metadata, const std::string& id, const Json::Value& charToLoad);

      Character(const Character& character) = delete;
      Character& operator=(const Character& character) = delete;

      Character(Character&& character);
      Character& operator=(Character&& character) = delete;

      ~Character();

      void initialize();

      /**
       * Serialize the character data into JSON output.
       *
       * @return A JSON object containing this character's serialized data.
       */
      Json::Value serialize() const;

      /**
       * @return The ID of the character.
       */
      std::string getId() const;

      /**
       * @return The name of the character.
       */
      std::string getName() const;

      /**
       * @return The ID of the spritesheet used to represent the character.
       */
      std::string getSpritesheetId() const;

      /**
       * @return The relative file path containing the picture of this character.
       * \todo The character portrait data should just contain an ID for the picture instead of a filepath.
       */
      std::string getPortraitPath() const;

      /**
       * @param The name of the requested stat attribute.
       *
       * @return Whether or not the character has the requested stat attribute.
       */
      bool hasStatAttribute(const std::string& attributeName) const;

      /**
       * @param The name of the requested stat attribute.
       *
       * @return The current value for the requested stat attribute.
       */
      int getStatAttribute(const std::string& attributeName) const;

      void setStatAttribute(const std::string& attributeName, int newValue);

      /**
       * @return The character's equipment information.
       */
      EquipData& getEquipment();

      /**
       * @return The list of the character's skills.
       */
      const SkillList& getSkillList() const;

      /**
       * @return The list of the character's skills.
       */
      bool hasSkill(UsableId skillId) const;

      /**
       * Marks a successful skill usage for the character.
       *
       * @param skillId The ID of the skill that was successfully used.
       */
      bool incrementSkillUsage(UsableId skillId);

      /**
       * Sets a piece of equipment, and changes the character's stat bonuses accordingly.
       *
       * @param slot The equipment slot to equip.
       * @param newEquipment The new piece of equipment to put into the slot.
       */
      bool equip(EquipSlot& slot, const Item* newEquipment);

      /**
       * Removes a piece equipment, and changes the character's stat bonuses accordingly.
       *
       * @param slot The equipment slot to unequip.
       */
      bool unequip(EquipSlot& slot);
};

#endif
