/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "PlayerDataSummary.h"
#include "PlayerData.h"
#include "Character.h"
#include <fstream>
#include "json.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_PLAYER

const char* PlayerDataSummary::CHARACTER_LIST_ELEMENT = "Roster";
const char* PlayerDataSummary::CHARACTER_ELEMENT = "Character";

PlayerDataSummary::PlayerDataSummary(const Metadata& metadata) :
   m_roster(metadata)
{
}

PlayerDataSummary& PlayerDataSummary::operator=(const PlayerData& playerData)
{
   /**
    * \todo Once the project moves to C++11, we can use STL smart pointers
    * to manage the members of these objects. After that, these serializations and reloads
    * can be replaced with proper assignment operators without bloating the code to
    * copy objects on the heap.
    */
   m_roster.load(playerData.getRoster()->serialize());

   return *this;
}

void PlayerDataSummary::load(const std::string& path)
{
   DEBUG("Loading save file %s", path.c_str());

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

   parseCharactersAndParty(jsonRoot);
}

void PlayerDataSummary::parseCharactersAndParty(Json::Value& rootElement)
{
   DEBUG("Loading character roster...");
   Json::Value& charactersElement = rootElement[PlayerDataSummary::CHARACTER_LIST_ELEMENT];
   m_roster.load(charactersElement);
}

const CharacterRoster* PlayerDataSummary::getRoster() const
{
   return &m_roster;
}
