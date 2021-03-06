/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Character.h"

#include "json.h"

#include <fstream>
#include <algorithm>

#include "Aspect.h"
#include "JsonUtils.h"
#include "Metadata.h"
#include "Skill.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_PLAYER

const char* Character::ARCHETYPE_ATTRIBUTE = "archetype";
const char* Character::NAME_ATTRIBUTE = "name";
const char* Character::SCHEMA_ATTRIBUTE = "schema";

const char* Character::BASE_STATS_ELEMENT = "baseStats";
const char* Character::CURRENT_STATS_ELEMENT = "currentStats";
const char* Character::SKILLS_ELEMENT = "Skills";
const char* Character::EQUIPMENT_ELEMENT = "Equipment";
const char* Character::ASPECTS_ELEMENT = "Aspects";

const char* Character::LEVEL_ATTRIBUTE = "level";
const char* Character::ASPECT_ATTRIBUTE = "aspect";

const char* Character::HP_ATTRIBUTE = "hp";
const char* Character::SP_ATTRIBUTE = "sp";

const char* Character::PORTRAIT_ELEMENT = "Portrait";
const char* Character::PORTRAIT_PATH_ATTRIBUTE = "path";

const char* Character::SPRITESHEET_ATTRIBUTE = "Spritesheet";

Json::Value Character::loadArchetype(const std::string& archetypeId)
{
   const std::string path = std::string("data/characters/") + archetypeId + ".edc";
   DEBUG("Loading archetype %s in file %s", archetypeId.c_str(), path.c_str());

   std::ifstream input(path.c_str());
   if(!input)
   {
      T_T("Failed to open character archetype file for reading.");
   }

   Json::Value jsonRoot;
   input >> jsonRoot;

   if(jsonRoot.isNull())
   {
      DEBUG("No root element was found.");
      T_T("Failed to parse character archetype data.");
   }

   return jsonRoot;
}

Character Character::createCharacter(Metadata& metadata, const std::string& id, int level)
{
   Character c(metadata, id, level);
   c.initialize();
   return c;
}

Character Character::loadCharacter(Metadata& metadata, const std::string& id, const Json::Value& charToLoad)
{
   Character c(metadata, id, charToLoad);
   c.initialize();
   return c;
}

Character::Character(Metadata& metadata, const std::string& id, int level) :
   m_metadata(metadata),
   m_id(id),
   m_level(level)
{
   parseArchetypeData(metadata, Character::loadArchetype(id));
}

Character::Character(Metadata& metadata, const std::string& id, const Json::Value& charToLoad) :
   m_metadata(metadata),
   m_id(id),
   m_selectedAspect(charToLoad.get(Character::ASPECT_ATTRIBUTE, 0).asInt()),
   m_level(std::max(charToLoad[Character::LEVEL_ATTRIBUTE].asInt(), 1))
{
   parseArchetypeData(metadata, charToLoad);
   parseCurrentStats(charToLoad);
   parseSkills(charToLoad);
}

Character::Character(Character&& character) = default;

Character::~Character() = default;

void Character::initialize()
{
   if(m_initialized)
   {
      return;
   }

   m_initialized = m_metadata.initializeCharacter(this, m_schema);
}

void Character::parseArchetypeData(const Metadata& metadata, const Json::Value& archetypeData)
{
   m_name = archetypeData[Character::NAME_ATTRIBUTE].asString();
   m_schema = archetypeData[Character::SCHEMA_ATTRIBUTE].asString();
   m_spritesheetId = archetypeData[Character::SPRITESHEET_ATTRIBUTE].asString();
   parsePortraitData(archetypeData);
   parseAspects(archetypeData);

   parseBaseStats(archetypeData);
   m_equipment.load(metadata, archetypeData[Character::EQUIPMENT_ELEMENT]);
}

void Character::parsePortraitData(const Json::Value& portraitDataContainer)
{
   const Json::Value& portraitData = portraitDataContainer[Character::PORTRAIT_ELEMENT];
   m_portraitPath = portraitData[Character::PORTRAIT_PATH_ATTRIBUTE].asString();
}

void Character::parseAspects(const Json::Value& aspectsDataContainer)
{
   const Json::Value& aspectsListNode = aspectsDataContainer[Character::ASPECTS_ELEMENT];
   if(aspectsListNode.isArray())
   {
      for(const auto& aspectNode : aspectsListNode)
      {
         m_archetypeAspects.push_back(Aspect::loadAspect((aspectNode).asString()));
      }
   }
}

void Character::parseBaseStats(const Json::Value& obj)
{
   const auto& baseStatsData = obj[Character::BASE_STATS_ELEMENT];
   m_baseStats = JsonUtils::toStlMap<int>(baseStatsData);
}

void Character::parseCurrentStats(const Json::Value& obj)
{
   const auto& currentStatsData = obj[Character::CURRENT_STATS_ELEMENT];
   m_currentStats = JsonUtils::toStlMap<int>(currentStatsData);
}

void Character::parseSkills(const Json::Value& skillsDataContainer)
{
   m_availableSkills.clear();
   m_skillUsage.clear();

   DEBUG("Loading skills...");
   const Json::Value& skillListJson = skillsDataContainer[Character::SKILLS_ELEMENT];

   for(Json::Value::const_iterator baseStatNameIter = skillListJson.begin(); baseStatNameIter != skillListJson.end(); ++baseStatNameIter)
   {
      UsableId skillId = std::stoi(baseStatNameIter.key().asCString());
      const std::pair<UsableId, unsigned int> skillUsage(skillId, (*baseStatNameIter).asUInt());
      DEBUG("Adding skill ID: %d", skillId);
      m_skillUsage.insert(skillUsage);
   }

   refreshAvailableSkills();
}

Json::Value Character::serialize() const
{
   Json::Value characterNode(Json::objectValue);

   if(!m_archetype.empty())
   {
      characterNode[Character::ARCHETYPE_ATTRIBUTE] = m_archetype;
   }

   characterNode[Character::LEVEL_ATTRIBUTE] = m_level;
   characterNode[Character::NAME_ATTRIBUTE] = m_name;

   characterNode[Character::SPRITESHEET_ATTRIBUTE] = m_spritesheetId;

   Json::Value portraitData(Json::objectValue);
   portraitData[Character::PORTRAIT_PATH_ATTRIBUTE] = m_portraitPath;
   characterNode[Character::PORTRAIT_ELEMENT] = portraitData;

   Json::Value aspectsData(Json::arrayValue);

   for(const auto& archetypeAspect : m_archetypeAspects)
   {
      aspectsData.append(archetypeAspect->getId());
   }

   characterNode[Character::ASPECTS_ELEMENT] = aspectsData;

   Json::Value baseStatsNode(Json::objectValue);
   for(const auto& iter : m_baseStats)
   {
      baseStatsNode[iter.first] = iter.second;
   }

   characterNode[Character::BASE_STATS_ELEMENT] = baseStatsNode;

   Json::Value currentStatsNode(Json::objectValue);
   for(const auto& iter : m_currentStats)
   {
      currentStatsNode[iter.first] = iter.second;
   }
   
   characterNode[Character::CURRENT_STATS_ELEMENT] = currentStatsNode;

   Json::Value skillsNode(Json::objectValue);
   for(const auto& iter : m_skillUsage)
   {
      skillsNode[std::to_string(iter.first)] = iter.second;
   }

   characterNode[Character::SKILLS_ELEMENT] = skillsNode;

   m_equipment.serialize(characterNode[Character::EQUIPMENT_ELEMENT]);

   return characterNode;
}

bool Character::hasStatAttribute(const std::string& attributeName) const
{
   return (m_currentStats.find(attributeName) != m_currentStats.end()) ||
      (m_baseStats.find(attributeName) != m_baseStats.end());
}

int Character::getStatAttribute(const std::string& attributeName) const
{
   const auto& currentAspect = m_archetypeAspects[m_selectedAspect];

   int stat;

   const auto& currentStatIterator = m_currentStats.find(attributeName);
   if(currentStatIterator != m_currentStats.end())
   {
      stat = currentStatIterator->second;
   }
   else
   {
      const auto& baseStatIterator = m_baseStats.find(attributeName);
      stat = baseStatIterator != m_baseStats.end() ?
         baseStatIterator->second : 0;
   }

   return stat + currentAspect->getAspectBonus(attributeName, m_level);
}

void Character::setStatAttribute(const std::string& attributeName, int newValue)
{
   m_currentStats[attributeName] = newValue;
}

std::string Character::getId() const
{
   return m_id;
}

std::string Character::getName() const
{
   return m_name;
}

std::string Character::getSpritesheetId() const
{
   return m_spritesheetId;
}

std::string Character::getPortraitPath() const
{
   return m_portraitPath;
}

const SkillList& Character::getSkillList() const
{
   return m_availableSkills;
}

void Character::refreshAvailableSkills()
{
   std::vector<UsableId> adeptSkills;
   for(const auto& iter : m_skillUsage)
   {
      const auto& skill = m_metadata.getSkill(iter.first);
      if(skill && iter.second >= skill->getAdeptitudeThreshold())
      {
         const auto& insertionPoint = std::lower_bound(adeptSkills.begin(), adeptSkills.end(), iter.first);
         adeptSkills.insert(insertionPoint, iter.first);
      }
   }

   const auto& currentAspect = m_archetypeAspects[m_selectedAspect];
   m_availableSkills = currentAspect->getAvailableSkills(adeptSkills);
}

bool Character::hasSkill(UsableId skillId) const
{
   const auto& skillIter = std::find(m_availableSkills.begin(), m_availableSkills.end(), skillId);
   return skillIter != m_availableSkills.end();
}

bool Character::incrementSkillUsage(UsableId skillId)
{
   if(!hasSkill(skillId))
   {
      // If the character doesn't have the skill
      // then it's not possible to use it
      return false;
   }

   SkillUsage::iterator iter = m_skillUsage.find(skillId);
   if(iter == m_skillUsage.end())
   {
      m_skillUsage[skillId] = 1;
   }
   else
   {
      ++(iter->second);
   }

   return true;
}

EquipData& Character::getEquipment()
{
   return m_equipment;
}

bool Character::equip(EquipSlot& slot, const Item* newEquipment)
{
   slot.equipped = newEquipment;
   return true;
}

bool Character::unequip(EquipSlot& slot)
{
   slot.equipped = nullptr;
   return true;
}
