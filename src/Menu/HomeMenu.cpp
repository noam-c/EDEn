/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "HomeMenu.h"
#include "MenuShell.h"

#include "CharacterDependentMenu.h"
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

#define DEBUG_FLAG DEBUG_MENU

HomeMenu::HomeMenu(GameContext& gameContext, PlayerData& playerData) :
   MenuState(gameContext, playerData, "HomeMenu"),
   m_homeViewModel(playerData)
{
   initialize();
}

HomeMenu::HomeMenu(GameContext& gameContext, PlayerData& playerData, std::shared_ptr<MenuShell> menuShell) :
   MenuState(gameContext, playerData, "HomeMenu", std::move(menuShell)),
   m_homeViewModel(playerData)
{
   initialize();
}

HomeMenu::~HomeMenu() = default;

void HomeMenu::initialize()
{
   m_paneDocument = m_menuShell->getRocketContext()->LoadDocument("data/gui/homepane.rml");
   if(m_paneDocument != nullptr)
   {
      m_bindings.bindAction(m_paneDocument, "characterGrid", "click", [this](Rocket::Core::Event& event) { characterClicked(event); });
   }

   m_sidebarOptions.push_back("Items");
   m_sidebarOptions.push_back("Equip");
   m_sidebarOptions.push_back("Status");
   m_sidebarOptions.push_back("Skills");
   m_sidebarOptions.push_back("Formation");
   m_sidebarOptions.push_back("Party Change");
   m_sidebarOptions.push_back("Options");
}

void HomeMenu::characterClicked(Rocket::Core::Event& event)
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
         DEBUG("Character click registered.");
         int characterIndex = rowElement->GetParentRelativeIndex();
         selectCharacter(characterIndex);
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
            pushCharacterIndependentMenu(optionIndex);
            break;
         case 3:
            m_selectedDestinationMenu = optionIndex;
            break;
         default:
            break;
      }
   }
}

void HomeMenu::pushCharacterDependentMenu(int optionIndex, int characterIndex)
{
   std::shared_ptr<CharacterDependentMenu> newState;
   switch(optionIndex)
   {
      case 3:
         newState = std::make_shared<SkillMenu>(m_gameContext, m_playerData, m_menuShell);
         DEBUG("Skill menu constructed");
         break;
      default:
         break;
   }

   if(newState)
   {
      newState->setCharacter(characterIndex);
      getExecutionStack()->pushState(newState);
   }
}

void HomeMenu::pushCharacterIndependentMenu(int optionIndex)
{
   std::shared_ptr<MenuState> newState;
   switch(optionIndex)
   {
      case 0:
         newState = std::make_shared<ItemMenu>(m_gameContext, m_playerData, m_menuShell);
         DEBUG("Item menu constructed.");
         break;
      default:
         break;
   }

   if(newState)
   {
      DEBUG("Pushing new menu state.");
      getExecutionStack()->pushState(newState);
   }
}

void HomeMenu::selectCharacter(int slotIndex)
{
   DEBUG("Character selected at slot %d", slotIndex);

   /**
    * \todo Add default destination menu here for when a player clicks
    *       on a character before clicking on a sidebar action.
    */
   if(m_selectedDestinationMenu >= 0)
   {
      pushCharacterDependentMenu(m_selectedDestinationMenu, slotIndex);
      DEBUG("Character-dependent menu state pushed onto stack.");
   }

   m_selectedDestinationMenu = -1;
}
