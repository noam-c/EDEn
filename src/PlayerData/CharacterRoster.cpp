/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "CharacterRoster.h"
#include <algorithm>
#include "json.h"

#include "Character.h"
#include "SaveGameItemNames.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

CharacterRoster::CharacterRoster() : partyLeader(NULL)
{
}

CharacterRoster::~CharacterRoster()
{

   for(std::map<std::string, Character*>::iterator iter = allCharacters.begin(); iter != allCharacters.end(); ++iter)
   {
      delete iter->second;
   }
}

Character* CharacterRoster::getPartyLeader() const
{
   return partyLeader;
}

Character* CharacterRoster::getCharacter(const std::string& characterName) const
{
   std::map<std::string, Character*>::const_iterator iter = allCharacters.find(characterName);
   return iter != allCharacters.end() ? iter->second : NULL;
}

const std::vector<Character*>& CharacterRoster::getParty() const
{
   return party;
}

void CharacterRoster::addToParty(Character* character)
{
   if (partyLeader == NULL)
   {
      partyLeader = character;
   }

   if (find(party.begin(), party.end(), character) == party.end())
   {
      party.push_back(character);
   }

   if (allCharacters.find(character->getName()) == allCharacters.end())
   {
      allCharacters[character->getName()] = character;
   }
}

void CharacterRoster::load(Json::Value& charactersElement)
{
   Json::Value& partyElement = charactersElement[PARTY_ELEMENT];

   int partySize = partyElement.size();

   if(partyElement.isArray() && partySize > 0)
   {
      DEBUG("Loading party...");
      for(int i = 0; i < partySize; ++i)
      {
         Json::Value characterNode = partyElement[i];
         DEBUG("Adding character %d...", i);
         Character* currCharacter = new Character(characterNode);

         std::string characterId = currCharacter->getId();
         allCharacters[characterId] = currCharacter;
         party.push_back(currCharacter);
      }
      DEBUG("Party loaded.");
   }
}

Json::Value CharacterRoster::serialize() const
{
   Json::Value charactersNode(Json::objectValue);
   Json::Value partyNode(Json::arrayValue);
   for(std::vector<Character*>::const_iterator iter = party.begin(); iter != party.end(); ++iter)
   {
      Character* character = *iter;
      partyNode.append(character->serialize());
   }

   charactersNode[PARTY_ELEMENT] = partyNode;
   return charactersNode;
}

void CharacterRoster::removeFromParty(Character *character)
{
   remove(party.begin(), party.end(), character) == party.end();

   if(party.empty())
   {
      partyLeader = NULL;
   }
}
