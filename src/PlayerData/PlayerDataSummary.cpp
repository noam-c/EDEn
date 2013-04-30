/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "PlayerDataSummary.h"
#include "PlayerData.h"
#include "Character.h"
#include <fstream>
#include "json.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

const char* PlayerDataSummary::CHARACTER_LIST_ELEMENT = "Characters";
const char* PlayerDataSummary::CHARACTER_ELEMENT = "Character";

PlayerDataSummary::PlayerDataSummary(const GameContext& gameContext) :
   gameContext(gameContext),
   roster(gameContext)
{
}

PlayerDataSummary::~PlayerDataSummary()
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
   roster.load(playerData.getRoster()->serialize());

   return *this;
}

const std::string& PlayerDataSummary::getFilePath() const
{
   return filePath;
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
   
   filePath = path;
}

void PlayerDataSummary::parseCharactersAndParty(Json::Value& rootElement)
{
   DEBUG("Loading character roster...");
   Json::Value& charactersElement = rootElement[PlayerDataSummary::CHARACTER_LIST_ELEMENT];
   roster.load(charactersElement);
}

const CharacterRoster* PlayerDataSummary::getRoster() const
{
   return &roster;
}