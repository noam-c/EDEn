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

#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"
#include "EdenRocketBindings.h"
#include "MenuShell.h"

#include "PlayerData.h"
#include "Character.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

HomeMenu::HomeMenu(ExecutionStack& executionStack, PlayerData& playerData) :
   GameState(executionStack),
   bindings(this),
   playerData(playerData),
   homeViewModel(*playerData.getRoster())
{
   initialize();
}

HomeMenu::HomeMenu(ExecutionStack& executionStack, PlayerData& playerData, MenuShell& menuShell) :
   GameState(executionStack),
   bindings(this),
   playerData(playerData),
   homeViewModel(*playerData.getRoster()),
   menuShell(menuShell)
{
   initialize();
}

void HomeMenu::initialize()
{
   homePaneDocument = menuShell.getContext()->LoadDocument("data/gui/homepane.rml");
   if(homePaneDocument != NULL)
   {
      homePaneDocument->Show();
   }
}

bool HomeMenu::step()
{
   if(finished) return false;

   bool done = false;

   waitForInputEvent(done);

   menuShell.getContext()->Update();

   return !done;
}

void HomeMenu::waitForInputEvent(bool& finishState)
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

void HomeMenu::draw()
{
   menuShell.getContext()->Render();
}

HomeMenu::~HomeMenu()
{
}
