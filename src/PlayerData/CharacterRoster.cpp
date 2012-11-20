/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "CharacterRoster.h"
#include <algorithm>
#include "json.h"

#include "MessagePipe.h"
#include "RosterUpdateMessage.h"

#include "Character.h"
#include "SaveGameItemNames.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

CharacterRoster::CharacterRoster() :
   messagePipe(NULL),
   partyLeader(NULL)
{
}

CharacterRoster::~CharacterRoster()
{
   deleteCharacterList();
}

void CharacterRoster::signalRosterUpdate()
{
   if(messagePipe != NULL)
   {
      messagePipe->sendMessage(RosterUpdateMessage());
   }
}

void CharacterRoster::bindMessagePipe(const messaging::MessagePipe* pipe)
{
   messagePipe = pipe;
}

void CharacterRoster::deleteCharacterList()
{
   DEBUG("Destroying character roster...");

   for(std::map<std::string, Character*>::iterator iter = allCharacters.begin(); iter != allCharacters.end(); ++iter)
   {
      DEBUG("Destroying character %s", (iter->first).c_str());
      delete iter->second;
      DEBUG("Character %s destroyed", (iter->first).c_str());
   }

   DEBUG("Character roster destroyed.");
}

void CharacterRoster::clear()
{
   partyLeader = NULL;
   party.clear();

   deleteCharacterList();
   allCharacters.clear();
}

Character* CharacterRoster::loadNewCharacter(const std::string& id)
{
   if(getCharacter(id) == NULL)
   {
      allCharacters[id] = new Character(id);
      signalRosterUpdate();
   }

   return allCharacters[id];
}

Character* CharacterRoster::getPartyLeader() const
{
   return partyLeader;
}

Character* CharacterRoster::getCharacter(const std::string& id) const
{
   std::map<std::string, Character*>::const_iterator iter = allCharacters.find(id);
   return iter != allCharacters.end() ? iter->second : NULL;
}

const std::vector<Character*>& CharacterRoster::getParty() const
{
   return party;
}

void CharacterRoster::addToParty(Character* character)
{
   if(character == NULL)
   {
      return;
   }

   if(partyLeader == NULL)
   {
      partyLeader = character;
   }

   if(std::find(party.begin(), party.end(), character) == party.end())
   {
      party.push_back(character);
   }

   if(allCharacters.find(character->getId()) == allCharacters.end())
   {
      allCharacters[character->getId()] = character;
   }

   signalRosterUpdate();
}

void CharacterRoster::load(const Json::Value& charactersElement)
{
   clear();

   const Json::Value& partyElement = charactersElement[PARTY_ELEMENT];

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

   signalRosterUpdate();
}

Json::Value CharacterRoster::serialize() const
{
   Json::Value charactersNode(Json::objectValue);
   Json::Value partyNode(Json::arrayValue);
   for(std::vector<Character*>::const_iterator iter = party.begin(); iter != party.end(); ++iter)
   {
      const Character* character = *iter;
      partyNode.append(character->serialize());
   }

   charactersNode[PARTY_ELEMENT] = partyNode;
   return charactersNode;
}

void CharacterRoster::removeFromParty(Character* character)
{
   if(party.size() == 1)
   {
      std::remove(party.begin(), party.end(), character);
      partyLeader = NULL;
   }
   else if(party.size() > 1 && partyLeader == character)
   {
      std::vector<Character*>::iterator iter = std::find(party.begin(), party.end(), character);
      partyLeader = *(iter + 1 != party.end() ? iter + 1 : party.begin());
      party.erase(iter);
   }

   signalRosterUpdate();
}
