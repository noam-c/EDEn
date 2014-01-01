/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "HomeMenu.h"
#include "MenuShell.h"

#include "CharacterDependentMenu.h"
#include "DataMenu.h"
#include "ItemMenu.h"
#include "SkillMenu.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "PlayerData.h"

#include "ExecutionStack.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

HomeMenu::HomeMenu(GameContext& gameContext) :
   MenuState(gameContext, "HomeMenu"),
   m_bindings(this),
   m_homeViewModel(getCurrentPlayerData()),
   m_selectedDestinationMenu(-1)
{
   initialize();
}

HomeMenu::HomeMenu(GameContext& gameContext, MenuShell* menuShell) :
   MenuState(gameContext, "HomeMenu", menuShell),
   m_bindings(this),
   m_homeViewModel(getCurrentPlayerData()),
   m_selectedDestinationMenu(-1)
{
   initialize();
}

HomeMenu::~HomeMenu()
{
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
         selectCharacter(characterIndex, m_menuShell);
      }
   }
}

void HomeMenu::sidebarClicked(int optionIndex)
{
   if(m_selectedDestinationMenu == -1)
   {
      switch(optionIndex)
      {
         case 0:
         case 7:
            pushCharacterIndependentMenu(optionIndex, m_menuShell);
            break;
         case 3:
            m_selectedDestinationMenu = optionIndex;
            break;
         default:
            break;
      }
   }
}

void HomeMenu::pushCharacterDependentMenu(int optionIndex, int characterIndex, MenuShell* menuShell)
{
   CharacterDependentMenu* newState = NULL;
   switch(optionIndex)
   {
      case 3:
         newState = new SkillMenu(m_gameContext, menuShell);
         DEBUG("Skill menu constructed");
         break;
      default:
         break;
   }
   
   if(newState != NULL)
   {
      newState->setCharacter(characterIndex);
      getExecutionStack()->pushState(newState);
   }
}

void HomeMenu::pushCharacterIndependentMenu(int optionIndex, MenuShell* menuShell)
{
   MenuState* newState = NULL;
   switch(optionIndex)
   {
      case 0:
         newState = new ItemMenu(m_gameContext, menuShell);
         DEBUG("Item menu constructed.");
         break;
      case 7:
         newState = new DataMenu(m_gameContext, menuShell);
         DEBUG("Data menu constructed.");
         break;
      default:
         break;
   }
   
   if(newState != NULL)
   {
      DEBUG("Pushing new menu state.");
      getExecutionStack()->pushState(newState);
   }
}

void HomeMenu::selectCharacter(int slotIndex, MenuShell* menuShell)
{
   DEBUG("Character selected at slot %d", slotIndex);
   
   /**
    * \todo Add default destination menu here for when a player clicks
    *       on a character before clicking on a sidebar action.
    */
   if(m_selectedDestinationMenu >= 0)
   {
      pushCharacterDependentMenu(m_selectedDestinationMenu, slotIndex, menuShell);
      DEBUG("Character-dependent menu state pushed onto stack.");
   }
   
   m_selectedDestinationMenu = -1;
}
