/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Character.h"
#include "SaveGameItemNames.h"
#include "json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

Character::Character(const std::string& name) : name(name)
{
}

Character::Character(Json::Value& charToLoad)
{
   name = charToLoad[NAME_ATTRIBUTE].asString();
   hp = charToLoad[HP_ATTRIBUTE].asInt();
   sp = charToLoad[SP_ATTRIBUTE].asInt();
   maxHP = charToLoad[MAX_HP_ATTRIBUTE].asInt();
   maxSP = charToLoad[MAX_SP_ATTRIBUTE].asInt();
   strength = charToLoad[STR_ATTRIBUTE].asInt();
   intelligence = charToLoad[INT_ATTRIBUTE].asInt();
   equipment.load(charToLoad["Equipment"]);

   parsePortraitData(charToLoad);
}

void Character::parsePortraitData(Json::Value& charToLoad)
{
   Json::Value& portraitData = charToLoad[PORTRAIT_ELEMENT];
   portraitPath = portraitData[PATH_ATTRIBUTE].asString();
}

void Character::serialize(Json::Value& characterSet) const
{
   Json::Value characterNode(Json::objectValue);
   
   characterNode[NAME_ATTRIBUTE] = name;
   characterNode[HP_ATTRIBUTE] = hp;
   characterNode[SP_ATTRIBUTE] = sp;
   characterNode[MAX_HP_ATTRIBUTE] = maxHP;
   characterNode[MAX_SP_ATTRIBUTE] = maxSP;
   characterNode[STR_ATTRIBUTE] = strength;
   characterNode[INT_ATTRIBUTE] = intelligence;
   equipment.serialize(characterNode["Equipment"]);
   
   serializePortraitData(characterNode);
   characterSet.append(characterNode);
}

void Character::serializePortraitData(Json::Value& characterNode) const
{
   Json::Value portraitNode(Json::objectValue);
   portraitNode[PATH_ATTRIBUTE] = portraitPath;
   characterNode[PORTRAIT_ELEMENT] = portraitNode;
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

std::string Character::getName() const
{
   return name;
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
