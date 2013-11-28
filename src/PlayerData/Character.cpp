/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Character.h"
#include "json.h"
#include <fstream>
#include <algorithm>
#include "Aspect.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

const char* Character::ID_ATTRIBUTE = "id";
const char* Character::ARCHETYPE_ATTRIBUTE = "archetype";
const char* Character::NAME_ATTRIBUTE = "name";

const char* Character::BASE_STATS_ELEMENT = "baseStats";
const char* Character::STATS_ELEMENT = "Stats";
const char* Character::SKILLS_ELEMENT = "Skills";
const char* Character::EQUIPMENT_ELEMENT = "Equipment";
const char* Character::ASPECTS_ELEMENT = "Aspects";

const char* Character::LEVEL_ATTRIBUTE = "level";
const char* Character::ASPECT_ATTRIBUTE = "aspect";

const char* Character::MAX_HP_ATTRIBUTE = "maxHP";
const char* Character::MAX_SP_ATTRIBUTE = "maxSP";
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

Character::Character(const Metadata& metadata, const std::string& id, int level) :
   m_id(id),
   m_selectedAspect(0),
   m_level(level)
{
   parseArchetypeData(metadata, Character::loadArchetype(id));

   // Start off the character with full HP/SP
   m_hp = getMaxHP();
   m_sp = getMaxSP();
}

Character::Character(const Metadata& metadata, const Json::Value& charToLoad)
{
   m_id = charToLoad[Character::ID_ATTRIBUTE].asString();
   m_selectedAspect = charToLoad.get(Character::ASPECT_ATTRIBUTE, 0).asInt();
   m_level = std::max(charToLoad[Character::LEVEL_ATTRIBUTE].asInt(), 1);

   parseArchetypeData(metadata, charToLoad);
   parseSkills(charToLoad);

   m_hp = charToLoad[Character::HP_ATTRIBUTE].asInt();
   m_sp = charToLoad[Character::SP_ATTRIBUTE].asInt();

   m_hp = std::min(m_hp, getMaxHP());
   m_sp = std::min(m_sp, getMaxSP());
}

Character::~Character()
{
   for(std::vector<Aspect*>::const_iterator iter = m_archetypeAspects.begin(); iter != m_archetypeAspects.end(); ++iter)
   {
      delete *iter;
   }
}

void Character::parseArchetypeData(const Metadata& metadata, const Json::Value& archetypeData)
{
   m_name = archetypeData[Character::NAME_ATTRIBUTE].asString();
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
   const Json::Value& aspectsData = aspectsDataContainer[Character::ASPECTS_ELEMENT];
   if(aspectsData.isArray())
   {
      for(Json::Value::iterator aspectIter = aspectsData.begin(); aspectIter != aspectsData.end(); ++aspectIter)
      {
         Aspect* aspect = Aspect::loadAspect((*aspectIter).asString());
         m_archetypeAspects.push_back(aspect);
      }
   }
}

void Character::parseBaseStats(const Json::Value& baseStatsDataContainer)
{
   const Json::Value& baseStatsData = baseStatsDataContainer[Character::BASE_STATS_ELEMENT];
   for(Json::Value::const_iterator baseStatNameIter = baseStatsData.begin(); baseStatNameIter != baseStatsData.end(); ++baseStatNameIter)
   {
      const std::pair<std::string, int> baseStat(baseStatNameIter.key().asString(), (*baseStatNameIter).asInt());
      m_baseStats.insert(baseStat);
   }
}

void Character::parseSkills(const Json::Value& skillsDataContainer)
{
   m_skills.clear();

   DEBUG("Loading skills...");
   const Json::Value& skillListJson = skillsDataContainer[Character::SKILLS_ELEMENT];

   for(int i = 0; i < skillListJson.size(); ++i)
   {
      int id = skillListJson[i].asInt();
      DEBUG("Adding skill ID: %d", id);
      m_skills.push_back(id);
   }
}

Json::Value Character::serialize() const
{
   Json::Value characterNode(Json::objectValue);
   
   if(!m_archetype.empty())
   {
      characterNode[Character::ARCHETYPE_ATTRIBUTE] = m_archetype;
   }

   characterNode[Character::ID_ATTRIBUTE] = m_id;
   characterNode[Character::LEVEL_ATTRIBUTE] = m_level;
   characterNode[Character::NAME_ATTRIBUTE] = m_name;

   characterNode[Character::SPRITESHEET_ATTRIBUTE] = m_spritesheetId;

   Json::Value portraitData(Json::objectValue);
   portraitData[Character::PORTRAIT_PATH_ATTRIBUTE] = m_portraitPath;
   characterNode[Character::PORTRAIT_ELEMENT] = portraitData;

   Json::Value aspectsData(Json::arrayValue);

   for(std::vector<Aspect*>::const_iterator iter = m_archetypeAspects.begin(); iter != m_archetypeAspects.end(); ++iter)
   {
      aspectsData.append((*iter)->getId());
   }

   characterNode[Character::ASPECTS_ELEMENT] = aspectsData;
   
   Json::Value baseStatsNode(Json::objectValue);
   for(std::map<std::string, int>::const_iterator iter = m_baseStats.begin(); iter != m_baseStats.end(); ++iter)
   {
      baseStatsNode[iter->first] = iter->second;
   }

   characterNode[Character::BASE_STATS_ELEMENT] = baseStatsNode;
   
   characterNode[Character::HP_ATTRIBUTE] = m_hp;
   characterNode[Character::SP_ATTRIBUTE] = m_sp;

   Json::Value skillsNode(Json::arrayValue);

   for(SkillList::const_iterator iter = m_skills.begin(); iter != m_skills.end(); ++iter)
   {
      skillsNode.append(*iter);
   }

   characterNode[Character::SKILLS_ELEMENT] = skillsNode;

   m_equipment.serialize(characterNode[Character::EQUIPMENT_ELEMENT]);
   
   return characterNode;
}

int Character::getStatAttribute(const std::string& attributeName) const
{
   const Aspect* currentAspect = m_archetypeAspects[m_selectedAspect];
   std::map<std::string, int>::const_iterator baseStatIterator = m_baseStats.find(attributeName);
   int baseStat = baseStatIterator != m_baseStats.end() ?
      baseStatIterator->second : 0;

   return baseStat + currentAspect->getAspectBonus(attributeName, m_level);
}

int Character::getMaxHP() const
{
   return getStatAttribute(Character::MAX_HP_ATTRIBUTE);
}

int Character::getMaxSP() const
{
   return getStatAttribute(Character::MAX_SP_ATTRIBUTE);
}

int Character::getHP() const
{
   return m_hp;
}

int Character::getSP() const
{
   return m_sp;
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
   return m_skills;
}

bool Character::hasSkill(UsableId skillId) const
{
   SkillList::const_iterator skillIter = std::find(m_skills.begin(), m_skills.end(), skillId);
   return skillIter != m_skills.end();
}

bool Character::addSkill(UsableId skillId)
{
   if(hasSkill(skillId))
   {
      // Skill was already added
      return false;
   }

   m_skills.push_back(skillId);
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
   slot.equipped = NULL;
   return true;
}
