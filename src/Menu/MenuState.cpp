/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MenuState.h"
#include "MenuShell.h"
#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"
#include "EdenRocketBindings.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

MenuState::MenuState(ExecutionStack& executionStack, const std::string& stateName) :
   GameState(executionStack, stateName),
   internalMenuShell(true)
{
   menuShell = new MenuShell(context);
}

MenuState::MenuState(ExecutionStack& executionStack, const std::string& stateName, MenuShell* menuShell) :
   GameState(executionStack, stateName, menuShell->getContext()),
   internalMenuShell(false),
   menuShell(menuShell)
{
}

MenuState::~MenuState()
{
   if(internalMenuShell)
   {
      delete menuShell;
   }
}

void MenuState::activate()
{
   GameState::activate();
   menuShell->changeMenuState(this);
}


bool MenuState::step()
{
   if(finished) return false;

   /* The menu shouldn't run too fast */
   SDL_Delay (1);

   bool done = false;

   waitForInputEvent(done);

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

void MenuState::draw()
{
}

std::vector<MenuShellOption> MenuState::getSidebarOptions()
{
   return sidebarOptions;
}

void MenuState::sidebarClicked(int optionIndex)
{
}

void MenuState::setSidebarEnabled(bool enabled)
{
   if(paneDocument != NULL)
   {
      paneDocument->SetClass("noSidebar", !enabled);
   }
}
