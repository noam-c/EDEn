/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MenuState.h"
#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"
#include "EdenRocketBindings.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

MenuState::MenuState(ExecutionStack& executionStack) :
   GameState(executionStack)
{
}

MenuState::MenuState(ExecutionStack& executionStack, MenuShell& menuShell) :
   GameState(executionStack), menuShell(menuShell)
{
}

MenuState::~MenuState()
{
}

void MenuState::activate()
{
   GameState::activate();
   menuShell.changeMenuState(this);
}


bool MenuState::step()
{
   if(finished) return false;

   bool done = false;

   waitForInputEvent(done);

   menuShell.getContext()->Update();

   return !done;
}

void MenuState::waitForInputEvent(bool& finishState)
{
   /* The menu shouldn't run too fast */
   SDL_Delay (1);

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

   RocketSDLInputMapping::handleSDLEvent(menuShell.getContext(), event);
}

void MenuState::draw()
{
   menuShell.getContext()->Render();
}

std::vector<MenuShellOption> MenuState::getSidebarOptions()
{
   return sidebarOptions;
}
