/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "MenuState.h"
#include "MenuShell.h"
#include "Scheduler.h"
#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"
#include "EdenRocketBindings.h"
#include "ShortcutBar.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

MenuState::MenuState(GameContext& gameContext, const std::string& stateName) :
   GameState(gameContext, GameState::MENU, stateName),
   m_internalMenuShell(true)
{
   m_menuShell = new MenuShell(m_rocketContext);
   m_menuShell->initializeShortcutBar(getCurrentPlayerData(), getScriptEngine(), getMetadata(), getStateType());
}

MenuState::MenuState(GameContext& gameContext, const std::string& stateName, MenuShell* menuShell) :
   GameState(gameContext, GameState::MENU, stateName, menuShell->getRocketContext()),
   m_internalMenuShell(false),
   m_menuShell(menuShell),
   m_shortcutBar(nullptr)
{
}

MenuState::~MenuState()
{
   if(m_internalMenuShell)
   {
      delete m_menuShell;
   }
   
   if(m_paneDocument != nullptr)
   {
      m_paneDocument->Close();
      m_paneDocument->RemoveReference();
   }
}

void MenuState::activate()
{
   GameState::activate();
   m_menuShell->changeMenuState(this);
   if(m_paneDocument != nullptr)
   {
      m_paneDocument->Show();
   }
}

void MenuState::deactivate()
{
   if(m_paneDocument != nullptr)
   {
      m_paneDocument->Hide();
   }
}


bool MenuState::step(long timePassed)
{
   if(m_finished) return false;

   m_menuShell->getScheduler()->runCoroutines(timePassed);
   bool done = false;

   waitForInputEvent(done);

   /* The menu shouldn't run too fast */
   SDL_Delay (1);

   return !done;
}

void MenuState::waitForInputEvent(bool& finishState)
{
   SDL_Event event;

   /* Check for events */
   SDL_PollEvent(&event);

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
      case SDL_QUIT:
      {
         finishState = true;
         return;
      }
      default:
      {
         break;
      }
   }

   handleEvent(event);
}

Scheduler* MenuState::getScheduler() const
{
   return m_menuShell->getScheduler();
}

void MenuState::draw()
{
}

std::vector<MenuShellOption> MenuState::getSidebarOptions()
{
   return m_sidebarOptions;
}

void MenuState::sidebarClicked(int optionIndex)
{
}

void MenuState::setSidebarEnabled(bool enabled)
{
   if(m_paneDocument != nullptr)
   {
      m_paneDocument->SetClass("noSidebar", !enabled);
   }
}
