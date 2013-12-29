/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "CharacterRoster.h"
#include <algorithm>
#include "json.h"

#include "MessagePipe.h"
#include "RosterUpdateMessage.h"

#include "Character.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

const char* CharacterRoster::PARTY_ELEMENT = "Party";

CharacterRoster::CharacterRoster(const Metadata& metadata) :
   m_metadata(metadata),
   m_messagePipe(NULL),
   m_partyLeader(NULL)
{
}

CharacterRoster::~CharacterRoster()
{
   deleteCharacterList();
}

void CharacterRoster::signalRosterUpdate()
{
   if(m_messagePipe != NULL)
   {
      m_messagePipe->sendMessage(RosterUpdateMessage());
   }
}

void CharacterRoster::bindMessagePipe(const messaging::MessagePipe* pipe)
{
   m_messagePipe = pipe;
}

void CharacterRoster::deleteCharacterList()
{
   DEBUG("Destroying character roster...");

   for(std::map<std::string, Character*>::iterator iter = m_allCharacters.begin(); iter != m_allCharacters.end(); ++iter)
   {
      DEBUG("Destroying character %s", (iter->first).c_str());
      delete iter->second;
      DEBUG("Character %s destroyed", (iter->first).c_str());
   }

   DEBUG("Character roster destroyed.");
}

void CharacterRoster::clear()
{
   m_partyLeader = NULL;
   m_party.clear();

   deleteCharacterList();
   m_allCharacters.clear();
}

Character* CharacterRoster::loadNewCharacter(const std::string& id)
{
   if(getCharacter(id) == NULL)
   {
      m_allCharacters[id] = new Character(m_metadata, id);
      signalRosterUpdate();
   }

   return m_allCharacters[id];
}

Character* CharacterRoster::getPartyLeader() const
{
   return m_partyLeader;
}

Character* CharacterRoster::getCharacter(const std::string& id) const
{
   std::map<std::string, Character*>::const_iterator iter = m_allCharacters.find(id);
   return iter != m_allCharacters.end() ? iter->second : NULL;
}

const std::vector<Character*>& CharacterRoster::getParty() const
{
   return m_party;
}

void CharacterRoster::addToParty(Character* character)
{
   if(character == NULL)
   {
      return;
   }

   if(m_partyLeader == NULL)
   {
      m_partyLeader = character;
   }

   if(std::find(m_party.begin(), m_party.end(), character) == m_party.end())
   {
      m_party.push_back(character);
   }

   if(m_allCharacters.find(character->getId()) == m_allCharacters.end())
   {
      m_allCharacters[character->getId()] = character;
   }

   signalRosterUpdate();
}

void CharacterRoster::load(const Json::Value& charactersElement)
{
   clear();

   const Json::Value& partyElement = charactersElement[CharacterRoster::PARTY_ELEMENT];

   int partySize = partyElement.size();

   if(partyElement.isArray() && partySize > 0)
   {
      DEBUG("Loading party...");
      for(int i = 0; i < partySize; ++i)
      {
         Json::Value characterNode = partyElement[i];
         DEBUG("Adding character %d...", i);
         Character* currCharacter = new Character(m_metadata, characterNode);

         std::string characterId = currCharacter->getId();
         m_allCharacters[characterId] = currCharacter;
         m_party.push_back(currCharacter);
      }

      if(m_partyLeader == NULL && m_party.size() > 0)
      {
         /**
          * \todo Persist the party leader in the save data and remove
          * this hardcoded party leader setting.
          */
         m_partyLeader = m_party[0];
      }

      DEBUG("Party loaded.");
   }

   signalRosterUpdate();
}

Json::Value CharacterRoster::serialize() const
{
   Json::Value charactersNode(Json::objectValue);
   Json::Value partyNode(Json::arrayValue);
   for(std::vector<Character*>::const_iterator iter = m_party.begin(); iter != m_party.end(); ++iter)
   {
      const Character* character = *iter;
      partyNode.append(character->serialize());
   }

   charactersNode[CharacterRoster::PARTY_ELEMENT] = partyNode;
   return charactersNode;
}

void CharacterRoster::removeFromParty(Character* character)
{
   if(m_party.size() == 1)
   {
      std::remove(m_party.begin(), m_party.end(), character);
      m_partyLeader = NULL;
   }
   else if(m_party.size() > 1 && m_partyLeader == character)
   {
      std::vector<Character*>::iterator iter = std::find(m_party.begin(), m_party.end(), character);
      m_partyLeader = *(iter + 1 != m_party.end() ? iter + 1 : m_party.begin());
      m_party.erase(iter);
   }

   signalRosterUpdate();
}
