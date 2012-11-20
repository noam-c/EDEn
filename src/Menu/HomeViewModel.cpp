/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "HomeViewModel.h"
#include "CharacterRoster.h"
#include "Character.h"

#include <sstream>

HomeViewModel::HomeViewModel(const CharacterRoster& characterRoster)
      : Rocket::Controls::DataSource("homeViewModel"),
        characterRoster(characterRoster)
{
}

HomeViewModel::~HomeViewModel()
{
}

void HomeViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   Character* character = characterRoster.getParty()[row_index];

   if (table == "party")
   {
      for (int i = 0; i < columns.size(); ++i)
      {
         if (columns[i] == "name")
         {
            row.push_back(character->getName().c_str());
         }
         else if (columns[i] == "portrait")
         {
            row.push_back(character->getPortraitPath().c_str());
         }
         else if (columns[i] == "HP")
         {
            std::ostringstream hpStringStream;

            hpStringStream << "HP: " << character->getHP() << '/' << character->getMaxHP();
            Rocket::Core::String hpString(hpStringStream.str().c_str());
            row.push_back(hpString);
         }
         else if (columns[i] == "SP")
         {
            std::ostringstream spStringStream;

            spStringStream << "SP: " << character->getSP() << '/' << character->getMaxSP();
            Rocket::Core::String spString(spStringStream.str().c_str());
            row.push_back(spString);
         }
      }
   }
}

int HomeViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "party")
   {
      return characterRoster.getParty().size();
   }

   return 0;
}
