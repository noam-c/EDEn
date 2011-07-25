#include "DataMenu.h"
#include "DataPane.h"
#include "PlayerData.h"
#include "ExecutionStack.h"
#include "MenuShell.h"
#include "ConfirmState.h"
#include <dirent.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

DataMenu::DataMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData) : MenuState(executionStack, menuShell), playerData(playerData)
{
   DataPane* pane = new DataPane(menuShell.getDimension());
   setMenuPane(pane);
   pane->setSaveGameSelectListener(this);
   refresh();
}

void DataMenu::refresh()
{
   struct dirent *entry;
   DIR *dp;
   std::map<std::string, PlayerData*> saveGames;
   
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
         
         saveGames[path] = data;
      }
   }
   
   closedir(dp);
   
   ((DataPane*)menuPane)->setSaveGames(saveGames);
}

void DataMenu::saveGameSelected(const std::string& path)
{
   selectedSavePath = path;
   executionStack.pushState(new ConfirmState(executionStack, top, *this, "Save Game?", path));
}

void DataMenu::yesClicked()
{
   playerData.save(selectedSavePath);
   selectedSavePath.clear();
   refresh();
}

void DataMenu::noClicked()
{
   selectedSavePath.clear();
}

DataMenu::~DataMenu()
{
}
