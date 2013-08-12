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

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

DataMenu::DataMenu(GameContext& gameContext, PlayerData& playerData) :
   MenuState(gameContext, playerData, "DataMenu"),
   m_bindings(this),
   m_dataViewModel(gameContext, playerData)
{
   initialize();
}

DataMenu::DataMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell) :
   MenuState(gameContext, "DataMenu", menuShell),
   m_bindings(this),
   m_dataViewModel(gameContext, playerData)
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
}

DataMenu::~DataMenu()
{
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Close();
      m_paneDocument->RemoveReference();
   }

   if(m_confirmSaveDocument != NULL)
   {
      m_confirmSaveDocument->Close();
      m_confirmSaveDocument->RemoveReference();
   }
}

void DataMenu::activate()
{
   MenuState::activate();
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Show();
   }
}

void DataMenu::deactivate()
{
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Hide();
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
   m_dataViewModel.saveToSlot(m_slotToSave);
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
