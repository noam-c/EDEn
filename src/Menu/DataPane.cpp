/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "DataPane.h"
#include "ItemData.h"
#include "Item.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "StringListModel.h"
#include "SaveGameModule.h"
#include <string>
#include "DebugUtils.h"
#include "ModuleSelectListener.h"

const int debugFlag = DEBUG_MENU;

DataPane::DataPane(const gcn::Rectangle& rect) : MenuPane(rect), moduleSelectListener(NULL)
{
   addActionListener(this);
}

void DataPane::setSaveGames(std::vector<PlayerData*> saveGames)
{
   clearModules();

   for(std::vector<PlayerData*>::iterator iter = saveGames.begin(); iter != saveGames.end(); ++iter)
   {
      saveGameModules.push_back(new SaveGameModule(**iter));
   }
   
   const int moduleHeight = 200;
   int moduleY = 0;

   for(std::vector<SaveGameModule*>::iterator iter = saveGameModules.begin(); iter != saveGameModules.end(); ++iter)
   {
      (*iter)->setHeight(moduleHeight);
      (*iter)->setWidth(getWidth());
      (*iter)->addActionListener(this);
      
      add(*iter, 0, moduleY);
      moduleY += moduleHeight;
   }
}

void DataPane::setModuleSelectListener(edwt::ModuleSelectListener* listener)
{
   moduleSelectListener = listener;
}

void DataPane::action(const gcn::ActionEvent& event)
{
   int numSaveGames = saveGameModules.size();
   for(int i = 0; i < numSaveGames; ++i)
   {
      if(saveGameModules[i] == event.getSource())
      {
         if(moduleSelectListener != NULL)
         {
            moduleSelectListener->moduleSelected(i, saveGameModules[i]->getActionEventId());
         }

         break;
      }
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
