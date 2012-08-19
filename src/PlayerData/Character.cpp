/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Character.h"
#include "SaveGameItemNames.h"
#include "json.h"
#include <fstream>

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

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

Character::Character(const std::string& id) : id(id)
{
   Json::Value archetypeData = Character::loadArchetype(id);
   name = archetypeData[NAME_ATTRIBUTE].asString();
   spritesheetId = archetypeData[SPRITESHEET_ATTRIBUTE].asString();
   parsePortraitData(archetypeData);

   parseStats(archetypeData);
   equipment.load(archetypeData["Equipment"]);

   // Start off the character with full HP/SP
   hp = maxHP;
   sp = maxSP;
}

Character::Character(Json::Value& charToLoad)
{
   id = charToLoad[ID_ATTRIBUTE].asString();

   std::string archetypeId = archetype = charToLoad[ARCHETYPE_ATTRIBUTE].asString();
   if(archetypeId.empty())
   {
      archetypeId = id;
   }

   Json::Value archetypeData = Character::loadArchetype(archetypeId);
   name = archetypeData[NAME_ATTRIBUTE].asString();
   spritesheetId = archetypeData[SPRITESHEET_ATTRIBUTE].asString();
   parsePortraitData(archetypeData);

   parseStats(charToLoad);
   equipment.load(charToLoad["Equipment"]);
}

Character::~Character()
{
}

void Character::parsePortraitData(Json::Value& portraitDataContainer)
{
   Json::Value& portraitData = portraitDataContainer[PORTRAIT_ELEMENT];
   portraitPath = portraitData[PATH_ATTRIBUTE].asString();
}

void Character::parseStats(Json::Value& statsDataContainer)
{
   Json::Value& statsData = statsDataContainer[STATS_ELEMENT];

   hp = statsData[HP_ATTRIBUTE].asInt();
   sp = statsData[SP_ATTRIBUTE].asInt();
   maxHP = statsData[MAX_HP_ATTRIBUTE].asInt();
   maxSP = statsData[MAX_SP_ATTRIBUTE].asInt();
   strength = statsData[STR_ATTRIBUTE].asInt();
   intelligence = statsData[INT_ATTRIBUTE].asInt();
}

Json::Value Character::serialize() const
{
   Json::Value characterNode(Json::objectValue);
   
   if(!archetype.empty())
   {
      characterNode[ARCHETYPE_ATTRIBUTE] = archetype;
   }

   characterNode[ID_ATTRIBUTE] = id;

   Json::Value statsNode(Json::objectValue);
   statsNode[HP_ATTRIBUTE] = hp;
   statsNode[SP_ATTRIBUTE] = sp;
   statsNode[MAX_HP_ATTRIBUTE] = maxHP;
   statsNode[MAX_SP_ATTRIBUTE] = maxSP;
   statsNode[STR_ATTRIBUTE] = strength;
   statsNode[INT_ATTRIBUTE] = intelligence;

   characterNode[STATS_ELEMENT] = statsNode;

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
