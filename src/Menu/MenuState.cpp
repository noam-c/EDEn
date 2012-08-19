/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MenuState.h"

#include "guichan.hpp"
#include "MenuShell.h"
#include "MenuPane.h"
#include "Container.h"
#include "TabbedArea.h"
#include <SDL.h>
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

MenuState::MenuState(ExecutionStack& executionStack, MenuShell& menuShell) : GameState(executionStack, &menuShell), menuShell(menuShell), menuPane(NULL)
{
}

void MenuState::setMenuPane(MenuPane* pane)
{
   menuPane = pane;
   menuShell.addPane(menuPane);
   menuShell.setActiveState(this);
}

void MenuState::activate()
{
   if(menuPane == NULL)
   {
      DEBUG("Missing menu pane for the menu! Please initialize it by calling setMenuPane in the subclass.");
      T_T("Missing menu pane for the menu!");
   }

   GameState::activate();
   menuShell.setActiveState(this);
   menuPane->setVisible(true);
}

bool MenuState::step()
{
   /* Don't run too fast */
   SDL_Delay (1);

   if(finished) return false;
   bool done = false;

   pollInputEvent(done);

   return !done;
}

void MenuState::pollInputEvent(bool& finishState)
{  
   /* Check for events */
   SDL_Event event;
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

      // If the menu pane didn't consume this event at all, then propagate to the generic input handling
      handleEvent(event);
   }
}

void MenuState::draw()
{
}

void MenuState::tabChanged(int index)
{
}

void MenuState::showPanel(MenuAction panelToShow)
{
}

MenuState::~MenuState()
{
   menuShell.removePane(menuPane);
   delete menuPane;
}
