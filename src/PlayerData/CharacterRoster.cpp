/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "CharacterRoster.h"
#include <algorithm>
#include "json.h"

#include "MessagePipe.h"
#include "RosterUpdateMessage.h"

#include "Character.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_PLAYER

const char* CharacterRoster::CHARACTERS_ELEMENT = "Characters";
const char* CharacterRoster::LEADER_ELEMENT = "Leader";
const char* CharacterRoster::PARTY_ELEMENT = "Party";

CharacterRoster::CharacterRoster(Metadata& metadata) :
   m_metadata(metadata),
   m_messagePipe(nullptr),
   m_partyLeader(nullptr)
{
}

void CharacterRoster::signalRosterUpdate()
{
   if(m_messagePipe)
   {
      m_messagePipe->sendMessage(RosterUpdateMessage());
   }
}

void CharacterRoster::bindMessagePipe(const messaging::MessagePipe* pipe)
{
   m_messagePipe = pipe;
}

void CharacterRoster::clear()
{
   m_partyLeader = nullptr;
   m_party.clear();
   m_allCharacters.clear();
}

Character* CharacterRoster::loadNewCharacter(const std::string& id)
{
   auto insertionResult = m_allCharacters.emplace(id, Character::createCharacter(m_metadata, id));

   auto characterIterator = insertionResult.first;
   bool success = insertionResult.second;
   if(success)
   {
      signalRosterUpdate();
   }

   return &characterIterator->second;
}

Character* CharacterRoster::getPartyLeader() const
{
   return m_partyLeader;
}

Character* CharacterRoster::getCharacter(const std::string& id)
{
   const auto& iter = m_allCharacters.find(id);
   return iter != m_allCharacters.end() ? &iter->second : nullptr;
}

bool CharacterRoster::characterExists(const std::string& id) const
{
   return m_allCharacters.find(id) != m_allCharacters.end();
}

const std::vector<Character*>& CharacterRoster::getParty() const
{
   return m_party;
}

void CharacterRoster::addToParty(Character* character)
{
   if(character == nullptr)
   {
      return;
   }

   if(m_partyLeader == nullptr)
   {
      m_partyLeader = character;
   }

   if(std::find(m_party.begin(), m_party.end(), character) == m_party.end())
   {
      m_party.push_back(character);
   }

   signalRosterUpdate();
}

void CharacterRoster::load(const Json::Value& rosterElement)
{
   clear();

   const auto& charactersElement = rosterElement[CharacterRoster::CHARACTERS_ELEMENT];
   const auto& partyLeaderElement = rosterElement[CharacterRoster::LEADER_ELEMENT];
   const auto& partyElement = rosterElement[CharacterRoster::PARTY_ELEMENT];

   if(charactersElement.isObject() && charactersElement.size() > 0)
   {
      for(auto iter = charactersElement.begin(); iter != charactersElement.end(); ++iter)
      {
         auto characterId = iter.key().asString();
         DEBUG("Adding character %s to roster...", characterId.c_str());
         Json::Value characterNode = *iter;
         m_allCharacters.emplace(characterId, Character::loadCharacter(m_metadata, characterId, characterNode));
      }
   }
   
   int partySize = partyElement.size();
   if(partyElement.isArray() && partySize > 0)
   {
      DEBUG("Loading party...");
      for(int i = 0; i < partySize; ++i)
      {
         if(!partyElement[i].isString())
         {
            continue;
         }

         auto characterId = partyElement[i].asString();
         auto result = m_allCharacters.find(characterId);

         if(result == m_allCharacters.end())
         {
            continue;
         }

         DEBUG("Adding character %s to party...", characterId.c_str());
         auto& currCharacter = result->second;
         m_party.push_back(&currCharacter);
         
         if(partyLeaderElement.isString() &&
            characterId == partyLeaderElement.asString())
         {
            DEBUG("Party leader %s designated.", characterId.c_str());
            m_partyLeader = &currCharacter;
         }
      }

      if(m_partyLeader == nullptr && m_party.size() > 0)
      {
         m_partyLeader = m_party[0];
      }

      DEBUG("Party loaded.");
   }

   signalRosterUpdate();
}

Json::Value CharacterRoster::serialize() const
{
   Json::Value rosterNode(Json::objectValue);
   Json::Value charactersNode(Json::objectValue);

   Json::Value partyNode(Json::arrayValue);
   for(const auto& character : m_party)
   {
      charactersNode[character->getId()] = character->serialize();
      partyNode.append(character->getId());
   }
   
   rosterNode[CharacterRoster::CHARACTERS_ELEMENT] = charactersNode;
   
   if(m_partyLeader)
   {
      rosterNode[CharacterRoster::LEADER_ELEMENT] = m_partyLeader->getId();
   }
   rosterNode[CharacterRoster::PARTY_ELEMENT] = partyNode;
   return rosterNode;
}

void CharacterRoster::removeFromParty(Character* character)
{
   if(m_party.size() == 1)
   {
      std::remove(m_party.begin(), m_party.end(), character);
      m_partyLeader = nullptr;
   }
   else if(m_party.size() > 1 && m_partyLeader == character)
   {
      auto iter = std::find(m_party.begin(), m_party.end(), character);
      m_partyLeader = *(iter + 1 != m_party.end() ? iter + 1 : m_party.begin());
      m_party.erase(iter);
   }

   signalRosterUpdate();
}
