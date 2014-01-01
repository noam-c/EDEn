/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "DataMenu.h"
#include "MenuShell.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "PlayerData.h"
#include "PlayerDataSummary.h"

#include "dirent.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

DataMenu::DataMenu(GameContext& gameContext) :
   MenuState(gameContext, "DataMenu"),
   m_bindings(this),
   m_dataViewModel(*this)
{
   initialize();
}

DataMenu::DataMenu(GameContext& gameContext, MenuShell* menuShell) :
   MenuState(gameContext, "DataMenu", menuShell),
   m_bindings(this),
   m_dataViewModel(*this)
{
   initialize();
}

void DataMenu::initialize()
{
   m_paneDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/datapane.rml");
   if(m_paneDocument != NULL)
   {
      m_bindings.bindAction(m_paneDocument, "saveGameGrid", "click", &DataMenu::saveGameClicked);
   }

   m_confirmSaveDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/dataconfirmsave.rml");
   if(m_confirmSaveDocument != NULL)
   {
      m_bindings.bindAction(m_confirmSaveDocument, "confirm", "click", &DataMenu::confirmClicked);
      m_bindings.bindAction(m_confirmSaveDocument, "cancel", "click", &DataMenu::cancelClicked);
   }

   m_slotToSave = -1;

   m_sidebarOptions.push_back("Items");
   m_sidebarOptions.push_back("Equip");
   m_sidebarOptions.push_back("Status");
   m_sidebarOptions.push_back("Skills");
   m_sidebarOptions.push_back("Formation");
   m_sidebarOptions.push_back("Party Change");
   m_sidebarOptions.push_back("Options");
   m_sidebarOptions.push_back("Data");
   
   refreshSaveGames();
}

DataMenu::~DataMenu()
{
   if(m_confirmSaveDocument != NULL)
   {
      m_confirmSaveDocument->Close();
      m_confirmSaveDocument->RemoveReference();
   }
}

void DataMenu::sidebarClicked(int optionIndex)
{
   switch(optionIndex)
   {
      default:
         break;
   }
}

void DataMenu::showConfirmDialog(int index)
{
   m_slotToSave = index;
   if (m_confirmSaveDocument != NULL)
   {
      m_confirmSaveDocument->Show(Rocket::Core::ElementDocument::MODAL);
   }
}

void DataMenu::hideConfirmDialog()
{
   m_slotToSave = -1;
   if(m_confirmSaveDocument != NULL)
   {
      // Need to call Show without focus flags first, to release modal focus :(
      m_confirmSaveDocument->Show();

      m_confirmSaveDocument->Hide();
   }
}

void DataMenu::confirmClicked(Rocket::Core::Event* event)
{
   saveToSlot(m_slotToSave);
   hideConfirmDialog();
}

void DataMenu::cancelClicked(Rocket::Core::Event* event)
{
   hideConfirmDialog();
}

void DataMenu::saveGameClicked(Rocket::Core::Event* event)
{
   Rocket::Core::Element* target = event->GetTargetElement();

   // Move up the DOM to the datagridrow item holding this element
   while(target->GetParentNode() != NULL && target->GetTagName() != "datagridrow")
   {
      target = target->GetParentNode();
   }

   if(target != NULL)
   {
      // If we found a row element, cast it and get its index
      Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
      if(rowElement != NULL)
      {
         int saveGameIndex = rowElement->GetParentRelativeIndex();
         showConfirmDialog(saveGameIndex);
      }
   }
}

void DataMenu::saveToSlot(int slotIndex)
{
   PlayerData& currentData = getCurrentPlayerData();
   currentData.save(m_saveGames[slotIndex].first);
   *(m_saveGames[slotIndex].second) = currentData;
   m_dataViewModel.refresh(slotIndex);
}

void DataMenu::clearSaveGameList()
{
   std::vector<std::pair<std::string, PlayerDataSummary*> >::iterator iter;
   for(iter = m_saveGames.begin(); iter != m_saveGames.end(); ++iter)
   {
      delete iter->second;
   }
   
   m_saveGames.clear();
}

void DataMenu::refreshSaveGames()
{
   clearSaveGameList();
   
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
         PlayerDataSummary* saveGameData = new PlayerDataSummary(getMetadata());
         
         /** \todo Extract HARDCODED path into a constant. */
         std::string path = "data/savegames/" + filename;
         saveGameData->load(path);
         
         m_saveGames.push_back(std::make_pair(path, saveGameData));
      }
   }
   
   closedir(dp);
}

const SaveGameList& DataMenu::getSaveGames() const
{
   return m_saveGames;
}
