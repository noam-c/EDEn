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

DataMenu::DataMenu(GameContext& gameContext, PlayerData& playerData, const SaveLocation& saveLocation) :
   MenuState(gameContext, playerData, "DataMenu"),
   m_saveLocation(saveLocation),
   m_dataViewModel(*this)
{
   initialize();
}

void DataMenu::initialize()
{
   m_paneDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/datapane.rml");
   if(m_paneDocument != nullptr)
   {
      m_bindings.bindAction(m_paneDocument, "saveGameGrid", "click", [this](Rocket::Core::Event& event) { saveGameClicked(event); });
   }

   m_confirmSaveDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/dataconfirmsave.rml");
   if(m_confirmSaveDocument != nullptr)
   {
      m_bindings.bindAction(m_confirmSaveDocument, "confirm", "click", [this](Rocket::Core::Event& event) { confirmClicked(event); });
      m_bindings.bindAction(m_confirmSaveDocument, "cancel", "click", [this](Rocket::Core::Event& event) { cancelClicked(event); });
   }

   m_slotToSave = -1;
   
   refreshSaveGames();
}

DataMenu::~DataMenu()
{
   if(m_confirmSaveDocument != nullptr)
   {
      m_confirmSaveDocument->Close();
      m_confirmSaveDocument->RemoveReference();
   }
}

void DataMenu::showConfirmDialog(int index)
{
   m_slotToSave = index;
   if (m_confirmSaveDocument != nullptr)
   {
      m_confirmSaveDocument->Show(Rocket::Core::ElementDocument::MODAL);
   }
}

void DataMenu::hideConfirmDialog()
{
   m_slotToSave = -1;
   if(m_confirmSaveDocument != nullptr)
   {
      // Need to call Show without focus flags first, to release modal focus :(
      m_confirmSaveDocument->Show();

      m_confirmSaveDocument->Hide();
   }
}

void DataMenu::confirmClicked(Rocket::Core::Event& event)
{
   saveToSlot(m_slotToSave);
   hideConfirmDialog();
}

void DataMenu::cancelClicked(Rocket::Core::Event& event)
{
   hideConfirmDialog();
}

void DataMenu::saveGameClicked(Rocket::Core::Event& event)
{
   Rocket::Core::Element* target = event.GetTargetElement();

   // Move up the DOM to the datagridrow item holding this element
   while(target->GetParentNode() != nullptr && target->GetTagName() != "datagridrow")
   {
      target = target->GetParentNode();
   }

   if(target != nullptr)
   {
      // If we found a row element, cast it and get its index
      Rocket::Controls::ElementDataGridRow* rowElement = dynamic_cast<Rocket::Controls::ElementDataGridRow*>(target);
      if(rowElement != nullptr)
      {
         int saveGameIndex = rowElement->GetParentRelativeIndex();
         showConfirmDialog(saveGameIndex);
      }
   }
}

void DataMenu::saveToSlot(int slotIndex)
{
   m_playerData.save(m_saveLocation, m_saveGames[slotIndex].first);
   *(m_saveGames[slotIndex].second) = m_playerData;
   m_dataViewModel.refresh(slotIndex);
}

void DataMenu::refreshSaveGames()
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
         auto saveGameData = std::unique_ptr<PlayerDataSummary>(new PlayerDataSummary(getMetadata()));
         
         /** \todo Extract HARDCODED path into a constant. */
         std::string path = "data/savegames/" + filename;
         saveGameData->load(path);
         
         m_saveGames.emplace_back(path, std::move(saveGameData));
      }
   }
   
   closedir(dp);
}

const SaveGameList& DataMenu::getSaveGames() const
{
   return m_saveGames;
}
