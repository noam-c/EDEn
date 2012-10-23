/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "DataViewModel.h"
#include "PlayerData.h"
#include "CharacterRoster.h"
#include "Character.h"

#include "dirent.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

DataViewModel::DataViewModel(PlayerData& playerData) :
      Rocket::Controls::DataSource("dataViewModel"),
      playerData(playerData)
{
   refreshSaveGames();
}

DataViewModel::~DataViewModel()
{
}

void DataViewModel::saveToSlot(int slotIndex)
{
   playerData.save(saveGames[slotIndex].first);
   *(saveGames[slotIndex].second) = playerData;
   NotifyRowChange("saveGames", slotIndex, 1);
}

void DataViewModel::clearSaveGames()
{
   std::vector<std::pair<std::string, PlayerData*> >::iterator iter;
   for(iter = saveGames.begin(); iter != saveGames.end(); ++iter)
   {
      delete iter->second;
   }

   saveGames.clear();
}

void DataViewModel::refreshSaveGames()
{
   clearSaveGames();

   struct dirent *entry;
   DIR *dp;

   /** \todo Extract HARDCODED path into a constant. */
   dp = opendir("data/savegames");
   if (dp == NULL)
   {
      T_T("Failed to open data/savegames for save game listing.");
   }

   while((entry = readdir(dp)))
   {
      std::string filename(entry->d_name);
      if(filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".edd")
      {
         PlayerData* data = new PlayerData();

         /** \todo Extract HARDCODED path into a constant. */
         std::string path = "data/savegames/" + filename;
         data->load(path);

         saveGames.push_back(std::make_pair(path, data));
      }
   }

   closedir(dp);
}

void DataViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   PlayerData* saveGameData = saveGames[row_index].second;
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
      return saveGames.size();
   }

   return 0;
}
