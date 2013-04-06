/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "HomeMenu.h"
#include "MenuShell.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "PlayerData.h"

#include "GameContext.h"
#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

HomeMenu::HomeMenu(GameContext& gameContext, PlayerData& playerData) :
   MenuState(gameContext, playerData, "HomeMenu"),
   bindings(this),
   playerData(playerData),
   homeViewModel(gameContext, playerData)
{
   initialize();
}

HomeMenu::HomeMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell) :
   MenuState(gameContext, "HomeMenu", menuShell),
   bindings(this),
   playerData(playerData),
   homeViewModel(gameContext, playerData)
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
   paneDocument = menuShell->getRocketContext()->LoadDocument("data/gui/homepane.rml");
   if(paneDocument != NULL)
   {
      bindings.bindAction(paneDocument, "characterGrid", "click", &HomeMenu::characterClicked);
   }

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

void HomeMenu::characterClicked(Rocket::Core::Event* event)
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
         DEBUG("Character click registered.");
         int characterIndex = rowElement->GetParentRelativeIndex();
         homeViewModel.selectCharacter(characterIndex, menuShell);
      }
   }
}

void HomeMenu::sidebarClicked(int optionIndex)
{
   homeViewModel.sidebarClicked(optionIndex, menuShell);
}
