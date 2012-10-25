/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "HomeMenu.h"
#include "DataMenu.h"
#include "MenuShell.h"

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "PlayerData.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

HomeMenu::HomeMenu(ExecutionStack& executionStack, PlayerData& playerData) :
   MenuState(executionStack, "HomeMenu"),
   bindings(this),
   playerData(playerData),
   homeViewModel(*playerData.getRoster())
{
   initialize();
}

HomeMenu::HomeMenu(ExecutionStack& executionStack, PlayerData& playerData, MenuShell* menuShell) :
   MenuState(executionStack, "HomeMenu", menuShell),
   bindings(this),
   playerData(playerData),
   homeViewModel(*playerData.getRoster())
{
   initialize();
}

HomeMenu::~HomeMenu()
{
   if(paneDocument != NULL)
   {
      paneDocument->Close();
      paneDocument->RemoveReference();
   }
}

void HomeMenu::initialize()
{
   paneDocument = menuShell->getContext()->LoadDocument("data/gui/homepane.rml");

   sidebarOptions.push_back("Items");
   sidebarOptions.push_back("Equip");
   sidebarOptions.push_back("Status");
   sidebarOptions.push_back("Skills");
   sidebarOptions.push_back("Formation");
   sidebarOptions.push_back("Party Change");
   sidebarOptions.push_back("Options");
   sidebarOptions.push_back("Data");
}

void HomeMenu::activate()
{
   MenuState::activate();
   if(paneDocument != NULL)
   {
      paneDocument->Show();
   }
}

void HomeMenu::deactivate()
{
   if(paneDocument != NULL)
   {
      paneDocument->Hide();
   }
}

void HomeMenu::sidebarClicked(int optionIndex)
{
   switch(optionIndex)
   {
      case 7:
         executionStack.pushState(new DataMenu(executionStack, playerData, menuShell));
         break;
      default:
         break;
   }
}
