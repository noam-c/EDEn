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
   m_bindings(this),
   m_homeViewModel(gameContext, playerData)
{
   initialize();
}

HomeMenu::HomeMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell) :
   MenuState(gameContext, "HomeMenu", menuShell),
   m_bindings(this),
   m_homeViewModel(gameContext, playerData)
{
   initialize();
}

HomeMenu::~HomeMenu()
{
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Close();
      m_paneDocument->RemoveReference();
   }
}

void HomeMenu::initialize()
{
   m_paneDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/homepane.rml");
   if(m_paneDocument != NULL)
   {
      m_bindings.bindAction(m_paneDocument, "characterGrid", "click", &HomeMenu::characterClicked);
   }

   m_sidebarOptions.push_back("Items");
   m_sidebarOptions.push_back("Equip");
   m_sidebarOptions.push_back("Status");
   m_sidebarOptions.push_back("Skills");
   m_sidebarOptions.push_back("Formation");
   m_sidebarOptions.push_back("Party Change");
   m_sidebarOptions.push_back("Options");
   m_sidebarOptions.push_back("Data");
}

void HomeMenu::activate()
{
   MenuState::activate();
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Show();
   }
}

void HomeMenu::deactivate()
{
   if(m_paneDocument != NULL)
   {
      m_paneDocument->Hide();
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
         m_homeViewModel.selectCharacter(characterIndex, m_menuShell);
      }
   }
}

void HomeMenu::sidebarClicked(int optionIndex)
{
   m_homeViewModel.sidebarClicked(optionIndex, m_menuShell);
}
