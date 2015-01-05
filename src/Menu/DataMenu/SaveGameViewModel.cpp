/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "SaveGameViewModel.h"
#include "PlayerData.h"
#include "PlayerDataSummary.h"
#include "CharacterRoster.h"
#include "Character.h"
#include "SaveMenu.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

SaveGameViewModel::SaveGameViewModel(SaveGameModel& model) :
      Rocket::Controls::DataSource("dataViewModel"),
      m_model(model)
{
}

void SaveGameViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   const auto& saveGameData = m_model.getSaveGame(row_index);
   const std::vector<Character*> characters = saveGameData->getRoster()->getParty();

   if (table == "saveGames")
   {
      for (int i = 0; i < columns.size(); ++i)
      {
         if (columns[i] == "character1")
         {
            if(characters.size() >= 1)
            {
               row.emplace_back(characters[0]->getPortraitPath().c_str());
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
               row.emplace_back(characters[1]->getPortraitPath().c_str());
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
               row.emplace_back(characters[2]->getPortraitPath().c_str());
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
               row.emplace_back(characters[4]->getPortraitPath().c_str());
            }
            else
            {
               row.push_back("");
            }
         }
      }
   }
}

int SaveGameViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "saveGames")
   {
      return m_model.getNumSaveGames();
   }

   return 0;
}

void SaveGameViewModel::refresh(int rowIndex)
{
   NotifyRowChange("saveGames", rowIndex, 1);
}
