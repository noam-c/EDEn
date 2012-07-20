/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "HomeMenu.h"
#include "GraphicsUtil.h"

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "Container.h"
#include "Label.h"
#include "TabbedArea.h"
#include "Tab.h"
#include "OpenGLTTF.h"
#include "StringListModel.h"
#include "ListBox.h"
#include "PlayerData.h"
#include "Character.h"

#include "MenuShell.h"
#include "ItemsMenu.h"
#include "StatusMenu.h"
#include "EquipMenu.h"
#include "DataMenu.h"
#include "HomePane.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

HomeMenu::HomeMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData) : MenuState(executionStack, menuShell), playerData(playerData), characterAction(NO_ACTION)
{
   HomePane* pane = new HomePane(playerData, menuShell.getDimension());
   setMenuPane(pane);
   pane->setModuleSelectListener(this);
}

bool HomeMenu::step()
{
   if(finished) return false;

   bool done = false;

   pollInputEvent(done);

   return !done;
}

void HomeMenu::pollInputEvent(bool& finishState)
{
   SDL_Event event;

   /* Check for events */
   if(SDL_PollEvent(&event))
   {
      switch (event.type)
      {
         case SDL_KEYDOWN:
         {
            switch(event.key.keysym.sym)
            {
               case SDLK_ESCAPE:
               {
                  finishState = true;
                  return;
               }
               default:
               {
                  break;
               }
            }
            break;
         }
         default:
         {
             break;
         }
      }

      // If the main HomeMenu didn't consume this event, then propagate to the generic input handling
      handleEvent(event);
   }
}

bool HomeMenu::isCharacterDependent(MenuAction action) const
{
   switch(action)
   {
      case EQUIP_PANEL:
      case STATUS_PANEL:
      case SKILLS_PANEL:
         return true;

      case FORMATION_PANEL:
      case PARTY_CHANGE_PANEL:
      case OPTIONS_PANEL:
      case DATA_PANEL:
      case ITEM_PANEL:
      default:
         return false;
   }
}

void HomeMenu::showPanel(MenuAction panelToShow)
{
   if(isCharacterDependent(panelToShow))
   {
      DEBUG("Character-dependent action selected");
      characterAction = panelToShow;
   }
   else
   {
      DEBUG("Character-independent action selected");
      
      MenuState* nextState;
      switch(panelToShow)
      {
         case ITEM_PANEL:
            nextState = new ItemsMenu(executionStack, menuShell, playerData);
            break;
         case DATA_PANEL:
         default:
            nextState = new DataMenu(executionStack, menuShell, playerData);
            break;
      }

      executionStack.pushState(nextState);
      menuPane->setVisible(false);
   }
}

void HomeMenu::moduleSelected(int index, const std::string& eventId)
{
   Character* character = playerData.getRoster()->getParty()[index];
   
   DEBUG("Character selected: %s", character->getName().c_str());
   
   MenuState* nextState;
   switch(characterAction)
   {
      case STATUS_PANEL:
         DEBUG("Creating Status menu panel...");
         nextState = new StatusMenu(executionStack, menuShell, playerData, character);
         break;
      case EQUIP_PANEL:
      default:
         DEBUG("Creating Equip menu panel...");
         nextState = new EquipMenu(executionStack, menuShell, playerData, character);
         break;
   }
   
   executionStack.pushState(nextState);
   menuPane->setVisible(false);
}

void HomeMenu::tabChanged(int index)
{
   Character* tabbedCharacter = playerData.getRoster()->getParty()[index - 1];
   executionStack.pushState(new StatusMenu(executionStack, menuShell, playerData, tabbedCharacter));
   menuPane->setVisible(false);
}

void HomeMenu::action(const gcn::ActionEvent& event)
{
   
}

HomeMenu::~HomeMenu()
{
}
