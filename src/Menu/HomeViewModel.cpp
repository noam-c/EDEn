/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "HomeViewModel.h"

#include "CharacterDependentMenu.h"
#include "DataMenu.h"
#include "ItemMenu.h"
#include "SkillMenu.h"

#include "GameContext.h"

#include "PlayerData.h"
#include "CharacterRoster.h"
#include "Character.h"

#include <sstream>

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

HomeViewModel::HomeViewModel(GameContext& gameContext, PlayerData& playerData) :
   Rocket::Controls::DataSource("homeViewModel"),
   m_gameContext(gameContext),
   m_playerData(playerData),
   m_selectedDestinationMenu(-1)
{
}

HomeViewModel::~HomeViewModel()
{
}

void HomeViewModel::pushCharacterDependentMenu(int optionIndex, int characterIndex, MenuShell* menuShell)
{
   CharacterDependentMenu* newState = NULL;
   switch(optionIndex)
   {
      case 3:
         newState = new SkillMenu(m_gameContext, m_playerData, menuShell);
         DEBUG("Skill menu constructed");
         break;
      default:
         break;
   }

   if(newState != NULL)
   {
      newState->setCharacter(characterIndex);
      m_gameContext.getExecutionStack().pushState(newState);
   }
}

void HomeViewModel::pushCharacterIndependentMenu(int optionIndex, MenuShell* menuShell)
{
   MenuState* newState = NULL;
   switch(optionIndex)
   {
      case 0:
         newState = new ItemMenu(m_gameContext, m_playerData, menuShell);
         DEBUG("Item menu constructed.");
         break;
      case 7:
         newState = new DataMenu(m_gameContext, m_playerData, menuShell);
         DEBUG("Data menu constructed.");
         break;
      default:
         break;
   }

   if(newState != NULL)
   {
      DEBUG("Pushing new menu state.");
      m_gameContext.getExecutionStack().pushState(newState);
   }
}

void HomeViewModel::selectCharacter(int slotIndex, MenuShell* menuShell)
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

void HomeViewModel::sidebarClicked(int optionIndex, MenuShell* menuShell)
{
   if(m_selectedDestinationMenu == -1)
   {
      switch(optionIndex)
      {
         case 0:
         case 7:
            pushCharacterIndependentMenu(optionIndex, menuShell);
            break;
         case 3:
            m_selectedDestinationMenu = optionIndex;
            break;
         default:
            break;
      }
   }
}

void HomeViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   Character* character = m_playerData.getRoster()->getParty()[row_index];

   if (table == "party")
   {
      for (int i = 0; i < columns.size(); ++i)
      {
         if (columns[i] == "name")
         {
            row.push_back(character->getName().c_str());
         }
         else if (columns[i] == "portrait")
         {
            row.push_back(character->getPortraitPath().c_str());
         }
         else if (columns[i] == "HP")
         {
            std::ostringstream hpStringStream;

            hpStringStream << "HP: " << character->getHP() << '/' << character->getMaxHP();
            Rocket::Core::String hpString(hpStringStream.str().c_str());
            row.push_back(hpString);
         }
         else if (columns[i] == "SP")
         {
            std::ostringstream spStringStream;

            spStringStream << "SP: " << character->getSP() << '/' << character->getMaxSP();
            Rocket::Core::String spString(spStringStream.str().c_str());
            row.push_back(spString);
         }
      }
   }
}

int HomeViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "party")
   {
      return m_playerData.getRoster()->getParty().size();
   }

   return 0;
}
