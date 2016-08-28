/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "HomeViewModel.h"

#include "PlayerData.h"
#include "CharacterRoster.h"
#include "Character.h"

#include <sstream>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_MENU

HomeViewModel::HomeViewModel(PlayerData& playerData) :
   Rocket::Controls::DataSource("homeViewModel"),
   m_playerData(playerData)
{
}

void HomeViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   Character* character = m_playerData.getRoster()->getParty()[row_index];

   if (table == "party")
   {
      for (int i = 0; i < columns.size(); ++i)
      {
         if (columns[i] == "name")
         {
            row.emplace_back(character->getName().c_str());
         }
         else if (columns[i] == "portrait")
         {
            row.emplace_back(character->getPortraitPath().c_str());
         }
         else if (columns[i] == "HP")
         {
            std::ostringstream hpStringStream;

            hpStringStream << "HP: " << character->getStatAttribute("hp") << '/' << character->getStatAttribute("maxHP");
            std::string hpString = hpStringStream.str();
            row.emplace_back(hpString.c_str());
         }
         else if (columns[i] == "SP")
         {
            std::ostringstream spStringStream;

            spStringStream << "SP: " << character->getStatAttribute("sp") << '/' << character->getStatAttribute("maxSP");
            std::string spString = spStringStream.str();
            row.emplace_back(spString.c_str());
         }
      }
   }
}

int HomeViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "party")
   {
      return m_playerData.getRoster()->getParty().size();
   }

   return 0;
}
