#include "DataPane.h"
#include "ItemData.h"
#include "Item.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "StringListModel.h"
#include "SaveGameModule.h"
#include <string>
#include "DebugUtils.h"
#include "SaveGameSelectListener.h"

const int debugFlag = DEBUG_MENU;

DataPane::DataPane(const gcn::Rectangle& rect) : MenuPane(rect), saveGameSelectListener(NULL)
{
}

void DataPane::setSaveGames(std::map<std::string, PlayerData*> saveGames)
{
   clearModules();

   for(std::map<std::string, PlayerData*>::iterator iter = saveGames.begin(); iter != saveGames.end(); ++iter)
   {
      SaveGameModule* module = new SaveGameModule(*(iter->second), iter->first);
      module->setSaveGameSelectListener(saveGameSelectListener);
      saveGameModules.push_back(module);
   }
   
   const int moduleHeight = 200;
   int moduleY = 0;

   for(std::vector<SaveGameModule*>::iterator iter = saveGameModules.begin(); iter != saveGameModules.end(); ++iter)
   {
      (*iter)->setHeight(moduleHeight);
      (*iter)->setWidth(getWidth());
      
      add(*iter, 0, moduleY);
      moduleY += moduleHeight;
   }
}

void DataPane::setSaveGameSelectListener(SaveGameSelectListener* listener)
{
   saveGameSelectListener = listener;
   for(std::vector<SaveGameModule*>::iterator iter = saveGameModules.begin(); iter != saveGameModules.end(); ++iter)
   {
      (*iter)->setSaveGameSelectListener(listener);
   }
}

void DataPane::clearModules()
{
   for(std::vector<SaveGameModule*>::iterator iter = saveGameModules.begin(); iter != saveGameModules.end(); ++iter)
   {
      delete *iter;
   }
   
   saveGameModules.clear();
}

DataPane::~DataPane()
{
   clearModules();
}
