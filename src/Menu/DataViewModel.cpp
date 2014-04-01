/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "DataViewModel.h"
#include "PlayerData.h"
#include "PlayerDataSummary.h"
#include "CharacterRoster.h"
#include "Character.h"
#include "DataMenu.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

DataViewModel::DataViewModel(DataMenu& dataMenu) :
      Rocket::Controls::DataSource("dataViewModel"),
      m_dataMenu(dataMenu)
{
}

DataViewModel::~DataViewModel()
{
}

void DataViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   const auto& saveGameData = m_dataMenu.getSaveGames()[row_index].second;
   const std::vector<Character*> characters = saveGameData->getRoster()->getParty();

   if (table == "saveGames")
   {
      for (int i = 0; i < columns.size(); ++i)
      {
         if (columns[i] == "character1")
         {
            if(characters.size() >= 1)
            {
               row.push_back(characters[0]->getPortraitPath().c_str());
            }
            else
            {
               row.push_back("");
            }
         }
         else if (columns[i] == "character2")
         {
            if(characters.size() >= 2)
            {
               row.push_back(characters[1]->getPortraitPath().c_str());
            }
            else
            {
               row.push_back("");
            }
         }
         else if (columns[i] == "character3")
         {
            if(characters.size() >= 3)
            {
               row.push_back(characters[2]->getPortraitPath().c_str());
            }
            else
            {
               row.push_back("");
            }
         }
         else if (columns[i] == "character4")
         {
            if(characters.size() >= 4)
            {
               row.push_back(characters[4]->getPortraitPath().c_str());
            }
            else
            {
               row.push_back("");
            }
         }
      }
   }
}

int DataViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "saveGames")
   {
      return m_dataMenu.getSaveGames().size();
   }

   return 0;
}

void DataViewModel::refresh(int rowIndex)
{
   NotifyRowChange("saveGames", rowIndex, 1);
}