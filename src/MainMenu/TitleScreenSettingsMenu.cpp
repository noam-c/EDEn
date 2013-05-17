/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "TitleScreenSettingsMenu.h"

#include <SDL.h>
#include <Rocket/Core.h>

#include "ResourceLoader.h"
#include "Scheduler.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_SETTINGS | DEBUG_TITLE;

TitleScreenSettingsMenu::TitleScreenSettingsMenu(GameContext& gameContext) :
   GameState(gameContext, GameState::TITLE, "TitleScreenSettingsMenu"),
   bindings(this)
{
   scheduler = new Scheduler();

   titleSettingsDocument = rocketContext->LoadDocument("data/gui/titleOptionsMenu.rml");

   if(titleSettingsDocument != NULL)
   {
      titleSettingsDocument->Show();
   }
}

TitleScreenSettingsMenu::~TitleScreenSettingsMenu()
{
   titleSettingsDocument->Close();
   titleSettingsDocument->RemoveReference();

   delete scheduler;
}

bool TitleScreenSettingsMenu::step(long timePassed)
{
   if(finished)
   {
      return false;
   }

   scheduler->runCoroutines(timePassed);
   bool done = false;

   waitForInputEvent(done);

   /* The menu shouldn't run too fast */
   SDL_Delay (1);

   return !done;
}

void TitleScreenSettingsMenu::waitForInputEvent(bool& finishState)
{
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
               finished = true;
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
         finished = true;
         return;
      }
      default:
      {
         break;
      }
   }

   handleEvent(event);
}

void TitleScreenSettingsMenu::draw()
{
}

Scheduler* TitleScreenSettingsMenu::getScheduler() const
{
   return scheduler;
}
