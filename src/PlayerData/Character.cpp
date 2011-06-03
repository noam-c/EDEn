#include "Character.h"
#include "SaveGameItemNames.h"
#include "tinyxml.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

Character::Character(const std::string& name) : name(name)
{
}

Character::Character(TiXmlElement* charToLoad)
{
   charToLoad->QueryStringAttribute(NAME_ATTRIBUTE, &name);
   charToLoad->QueryIntAttribute(HP_ATTRIBUTE, &hp);
   charToLoad->QueryIntAttribute(SP_ATTRIBUTE, &sp);
   charToLoad->QueryIntAttribute(MAX_HP_ATTRIBUTE, &maxHP);
   charToLoad->QueryIntAttribute(MAX_SP_ATTRIBUTE, &maxSP);
   charToLoad->QueryIntAttribute(STR_ATTRIBUTE, &strength);
   charToLoad->QueryIntAttribute(INT_ATTRIBUTE, &intelligence);

   parsePortraitData(charToLoad);
}

void Character::parsePortraitData(TiXmlElement* charToLoad)
{
   TiXmlElement* portraitData = charToLoad->FirstChild(PORTRAIT_ELEMENT)->ToElement();
   portraitData->QueryStringAttribute(PATH_ATTRIBUTE, &portraitPath);
}

void Character::serialize(TiXmlElement& characterSet)
{
   TiXmlElement characterNode(CHARACTER_ELEMENT);

   characterNode.SetAttribute(NAME_ATTRIBUTE, name);
   characterNode.SetAttribute(HP_ATTRIBUTE, hp);
   characterNode.SetAttribute(SP_ATTRIBUTE, sp);
   characterNode.SetAttribute(MAX_HP_ATTRIBUTE, maxHP);
   characterNode.SetAttribute(MAX_SP_ATTRIBUTE, maxSP);
   characterNode.SetAttribute(STR_ATTRIBUTE, strength);
   characterNode.SetAttribute(INT_ATTRIBUTE, intelligence);

   characterSet.InsertEndChild(characterNode);
}

void Character::serializePortraitData(TiXmlElement& characterNode)
{
   TiXmlElement portraitNode(PORTRAIT_ELEMENT);

   portraitNode.SetAttribute(PATH_ATTRIBUTE, portraitPath);

   characterNode.InsertEndChild(portraitNode);
}

int Character::getMaxHP()
{
   return maxHP;
}

int Character::getMaxSP()
{
   return maxSP;
}

int Character::getHP()
{
   return hp;
}

int Character::getSP()
{
   return sp;
}

int Character::getStrength()
{
   return strength;
}

int Character::getIntelligence()
{
   return intelligence;
}

int Character::getVitality()
{

   return vitality;
}

int Character::getReflex()
{
   return reflex;
}

int Character::getFocus()
{
   return focus;
}

int Character::getEndurance()
{
   return endurance;
}

int Character::getAgility()
{
   return agility;
}

std::string Character::getName()
{
   return name;
}

std::string Character::getPortraitPath()
{
   return portraitPath;
}