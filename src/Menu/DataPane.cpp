#include "DataPane.h"
#include "ItemData.h"
#include "Item.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "StringListModel.h"
#include "SaveGameModule.h"
#include <string>
#include <dirent.h>
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

DataPane::DataPane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
   struct dirent *entry;
   DIR *dp;
   std::vector<PlayerData*> files;
   
   /** \todo Extract HARDCODED path into a constant. */
   dp = opendir("data/savegames");
   if (dp == NULL)
   {
      T_T("opendir");
   }
   
   while((entry = readdir(dp)))
   {
      std::string filename(entry->d_name);
      if(filename.length() > 4 && filename.substr(filename.length() - 4, 4) == ".edd")
      {
         PlayerData* data = new PlayerData();

         /** \todo Extract HARDCODED path into a constant. */
         data->load("data/savegames/" + filename);
         
         saveGames.push_back(data);
         saveGameModules.push_back(new SaveGameModule(*data));
      }
   }
   
   closedir(dp);

   const int moduleHeight = 200;
   for(unsigned int i = 0; i < saveGameModules.size(); ++i)
   {
      saveGameModules[i]->setHeight(moduleHeight);
      saveGameModules[i]->setWidth(rect.width);
      
      add(saveGameModules[i], 0, i * moduleHeight);
   }
}

DataPane::~DataPane()
{
   
}
