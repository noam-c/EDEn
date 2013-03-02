/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Character.h"
#include "json.h"
#include <fstream>
#include <algorithm>

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

const char* Character::ID_ATTRIBUTE = "id";
const char* Character::ARCHETYPE_ATTRIBUTE = "archetype";
const char* Character::NAME_ATTRIBUTE = "name";

const char* Character::STATS_ELEMENT = "Stats";
const char* Character::SKILLS_ELEMENT = "Skills";

const char* Character::MAX_HP_ATTRIBUTE = "maxHP";
const char* Character::HP_ATTRIBUTE = "hp";
const char* Character::MAX_SP_ATTRIBUTE = "maxSP";
const char* Character::SP_ATTRIBUTE = "sp";
const char* Character::STR_ATTRIBUTE = "str";
const char* Character::INT_ATTRIBUTE = "int";

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
      T_T("Failed to open save game file for reading.");
   }

   Json::Value jsonRoot;
   input >> jsonRoot;

   if(jsonRoot.isNull())
   {
      DEBUG("Unexpected root element name.");
      T_T("Failed to parse save data.");
   }

   return jsonRoot;
}

Character::Character(const GameContext& gameContext, const std::string& id) :
   id(id)
{
   Json::Value archetypeData = Character::loadArchetype(id);
   name = archetypeData[Character::NAME_ATTRIBUTE].asString();
   spritesheetId = archetypeData[Character::SPRITESHEET_ATTRIBUTE].asString();
   parsePortraitData(archetypeData);

   parseStats(archetypeData);
   equipment.load(gameContext, archetypeData["Equipment"]);

   // Start off the character with full HP/SP
   hp = maxHP;
   sp = maxSP;
}

Character::Character(const GameContext& gameContext, Json::Value& charToLoad)
{
   id = charToLoad[Character::ID_ATTRIBUTE].asString();

   std::string archetypeId = archetype = charToLoad[Character::ARCHETYPE_ATTRIBUTE].asString();
   if(archetypeId.empty())
   {
      archetypeId = id;
   }

   Json::Value archetypeData = Character::loadArchetype(archetypeId);
   name = archetypeData[Character::NAME_ATTRIBUTE].asString();
   spritesheetId = archetypeData[Character::SPRITESHEET_ATTRIBUTE].asString();
   parsePortraitData(archetypeData);

   parseStats(charToLoad);
   equipment.load(gameContext, charToLoad["Equipment"]);
   parseSkills(charToLoad);
}

Character::~Character()
{
}

void Character::parsePortraitData(Json::Value& portraitDataContainer)
{
   Json::Value& portraitData = portraitDataContainer[Character::PORTRAIT_ELEMENT];
   portraitPath = portraitData[Character::PORTRAIT_PATH_ATTRIBUTE].asString();
}

void Character::parseStats(Json::Value& statsDataContainer)
{
   Json::Value& statsData = statsDataContainer[Character::STATS_ELEMENT];

   hp = statsData[Character::HP_ATTRIBUTE].asInt();
   sp = statsData[Character::SP_ATTRIBUTE].asInt();
   maxHP = statsData[Character::MAX_HP_ATTRIBUTE].asInt();
   maxSP = statsData[Character::MAX_SP_ATTRIBUTE].asInt();
   strength = statsData[Character::STR_ATTRIBUTE].asInt();
   intelligence = statsData[Character::INT_ATTRIBUTE].asInt();
}

void Character::parseSkills(Json::Value& skillsDataContainer)
{
   skills.clear();

   DEBUG("Loading skills...");
   Json::Value& skillListJson = skillsDataContainer[Character::SKILLS_ELEMENT];

   for(int i = 0; i < skillListJson.size(); ++i)
   {
      int id = skillListJson[i].asInt();
      DEBUG("Adding skill ID: %d", id);
      skills.push_back(id);
   }
}

Json::Value Character::serialize() const
{
   Json::Value characterNode(Json::objectValue);
   
   if(!archetype.empty())
   {
      characterNode[Character::ARCHETYPE_ATTRIBUTE] = archetype;
   }

   characterNode[Character::ID_ATTRIBUTE] = id;

   Json::Value statsNode(Json::objectValue);
   statsNode[Character::HP_ATTRIBUTE] = hp;
   statsNode[Character::SP_ATTRIBUTE] = sp;
   statsNode[Character::MAX_HP_ATTRIBUTE] = maxHP;
   statsNode[Character::MAX_SP_ATTRIBUTE] = maxSP;
   statsNode[Character::STR_ATTRIBUTE] = strength;
   statsNode[Character::INT_ATTRIBUTE] = intelligence;

   characterNode[Character::STATS_ELEMENT] = statsNode;

   Json::Value skillsNode(Json::arrayValue);

   for(SkillList::const_iterator iter = skills.begin(); iter != skills.end(); ++iter)
   {
      skillsNode.append(*iter);
   }

   characterNode[Character::SKILLS_ELEMENT] = skillsNode;

   equipment.serialize(characterNode["Equipment"]);
   
   return characterNode;
}

int Character::getMaxHP() const
{
   return maxHP;
}

int Character::getMaxSP() const
{
   return maxSP;
}

int Character::getHP() const
{
   return hp;
}

int Character::getSP() const
{
   return sp;
}

int Character::getStrength() const
{
   return strength;
}

int Character::getIntelligence() const
{
   return intelligence;
}

int Character::getVitality() const
{

   return vitality;
}

int Character::getReflex() const
{
   return reflex;
}

int Character::getFocus() const
{
   return focus;
}

int Character::getEndurance() const
{
   return endurance;
}

int Character::getAgility() const
{
   return agility;
}

std::string Character::getId() const
{
   return id;
}

std::string Character::getName() const
{
   return name;
}

std::string Character::getSpritesheetId() const
{
   return spritesheetId;
}

std::string Character::getPortraitPath() const
{
   return portraitPath;
}

const SkillList& Character::getSkillList() const
{
   return skills;
}

bool Character::hasSkill(UsableId skillId) const
{
   SkillList::const_iterator skillIter = std::find(skills.begin(), skills.end(), skillId);
   return skillIter != skills.end();
}

bool Character::addSkill(UsableId skillId)
{
   if(hasSkill(skillId))
   {
      // Skill was already added
      return false;
   }

   skills.push_back(skillId);
   return true;
}

EquipData& Character::getEquipment()
{
   return equipment;
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
