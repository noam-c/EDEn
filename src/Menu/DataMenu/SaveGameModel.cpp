/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "SaveGameModel.h"

#include "SaveMenu.h"

#include "PlayerData.h"
#include "PlayerDataSummary.h"

#include "dirent.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

SaveGameModel::SaveGameModel(SaveMenu& saveMenu, const Metadata& metadata) :
   m_saveMenu(saveMenu),
   m_metadata(metadata)
{
   refreshSaveGames();
}

void SaveGameModel::saveToSlot(const PlayerData& playerData, const SaveLocation& saveLocation, int slotIndex)
{
   playerData.save(saveLocation, m_saveGames[slotIndex].first);
   *(m_saveGames[slotIndex].second) = playerData;
   m_saveMenu.refresh(slotIndex);
}

void SaveGameModel::refreshSaveGames()
{
   m_saveGames.clear();
   
   struct dirent *entry;
   DIR *dp;
   
   /** \todo Extract HARDCODED path into a constant. */
   dp = opendir("data/savegames");
   if (dp == nullptr)
   {
      T_T("Failed to open data/savegames for save game listing.");
   }
   
   while((entry = readdir(dp)))
   {
      std::string filename(entry->d_name);
      if(filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".edd")
      {
         auto saveGameData = std::unique_ptr<PlayerDataSummary>(new PlayerDataSummary(m_metadata));
         
         /** \todo Extract HARDCODED path into a constant. */
         std::string path = "data/savegames/" + filename;
         saveGameData->load(path);
         
         m_saveGames.emplace_back(path, std::move(saveGameData));
      }
   }
   
   closedir(dp);
}

const std::unique_ptr<PlayerDataSummary>& SaveGameModel::getSaveGame(size_t index) const
{
   return m_saveGames[index].second;
}

size_t SaveGameModel::getNumSaveGames() const
{
   return m_saveGames.size();
}
