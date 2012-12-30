/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
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
   MenuState(gameContext, "DataMenu"),
   bindings(this),
   playerData(playerData),
   dataViewModel(playerData)
{
   initialize();
}

DataMenu::DataMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell) :
   MenuState(gameContext, "DataMenu", menuShell),
   bindings(this),
   playerData(playerData),
   dataViewModel(playerData)
{
   initialize();
}

void DataMenu::initialize()
{
   paneDocument = menuShell->getRocketContext()->LoadDocument("data/gui/datapane.rml");
   if(paneDocument != NULL)
   {
      bindings.bindAction(paneDocument, "saveGameGrid", "click", &DataMenu::saveGameClicked);
   }

   confirmSaveDocument = menuShell->getRocketContext()->LoadDocument("data/gui/dataconfirmsave.rml");
   if(confirmSaveDocument != NULL)
   {
      bindings.bindAction(confirmSaveDocument, "confirm", "click", &DataMenu::confirmClicked);
      bindings.bindAction(confirmSaveDocument, "cancel", "click", &DataMenu::cancelClicked);
   }

   slotToSave = -1;

   sidebarOptions.push_back("Items");
   sidebarOptions.push_back("Equip");
   sidebarOptions.push_back("Status");
   sidebarOptions.push_back("Skills");
   sidebarOptions.push_back("Formation");
   sidebarOptions.push_back("Party Change");
   sidebarOptions.push_back("Options");
   sidebarOptions.push_back("Data");
}

DataMenu::~DataMenu()
{
   if(paneDocument != NULL)
   {
      paneDocument->Close();
      paneDocument->RemoveReference();
   }

   if(confirmSaveDocument != NULL)
   {
      confirmSaveDocument->Close();
      confirmSaveDocument->RemoveReference();
   }
}

void DataMenu::activate()
{
   MenuState::activate();
   if(paneDocument != NULL)
   {
      paneDocument->Show();
   }
}

void DataMenu::deactivate()
{
   if(paneDocument != NULL)
   {
      paneDocument->Hide();
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
   slotToSave = index;
   if (confirmSaveDocument != NULL)
   {
      confirmSaveDocument->Show(Rocket::Core::ElementDocument::MODAL);
   }
}

void DataMenu::hideConfirmDialog()
{
   slotToSave = -1;
   if(confirmSaveDocument != NULL)
   {
      // Need to call Show without focus flags first, to release modal focus :(
      confirmSaveDocument->Show();

      confirmSaveDocument->Hide();
   }
}

void DataMenu::confirmClicked(Rocket::Core::Event* event)
{
   dataViewModel.saveToSlot(slotToSave);
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
