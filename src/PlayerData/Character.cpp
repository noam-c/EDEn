#include "Character.h"
#include "SaveGameItemNames.h"
#include "tinyxml.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

Character::Character(std::string name) : name(name)
{
}

Character::Character(TiXmlElement* charToLoad)
{
   charToLoad->QueryStringAttribute(NAME_ATTRIBUTE, &name);
   charToLoad->QueryIntAttribute(HP_ATTRIBUTE, &characterHP);
   charToLoad->QueryIntAttribute(SP_ATTRIBUTE, &characterSP);
   charToLoad->QueryIntAttribute(STR_ATTRIBUTE, &characterStrength);
   charToLoad->QueryIntAttribute(INT_ATTRIBUTE, &characterIntelligence);
}

void Character::serialize(TiXmlElement& characterSet)
{
   TiXmlElement characterNode(CHARACTER_ELEMENT);

   characterNode.SetAttribute(NAME_ATTRIBUTE, name);
   characterNode.SetAttribute(HP_ATTRIBUTE, characterHP);
   characterNode.SetAttribute(SP_ATTRIBUTE, characterSP);
   characterNode.SetAttribute(STR_ATTRIBUTE, characterStrength);
   characterNode.SetAttribute(INT_ATTRIBUTE, characterIntelligence);

   characterSet.InsertEndChild(characterNode);
}

std::string Character::getName()
{
   return name;
}