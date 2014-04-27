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
   m_messagePipe(nullptr),
   m_partyLeader(nullptr)
{
}

CharacterRoster::~CharacterRoster()
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
   auto insertionResult = m_allCharacters.emplace(
                             std::piecewise_construct,
                             std::forward_as_tuple(id),
                             std::forward_as_tuple(m_metadata, id));

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
         std::string characterId = characterNode[Character::ID_ATTRIBUTE].asString();
         auto result = m_allCharacters.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(characterId),
                                 std::forward_as_tuple(m_metadata, characterId, characterNode));

         auto& currCharacter = result.first->second;
         m_party.push_back(&currCharacter);
      }

      if(m_partyLeader == nullptr && m_party.size() > 0)
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
   for(const auto& character : m_party)
   {
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
      m_partyLeader = nullptr;
   }
   else if(m_party.size() > 1 && m_partyLeader == character)
   {
      std::vector<Character*>::iterator iter = std::find(m_party.begin(), m_party.end(), character);
      m_partyLeader = *(iter + 1 != m_party.end() ? iter + 1 : m_party.begin());
      m_party.erase(iter);
   }

   signalRosterUpdate();
}
