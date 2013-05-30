/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "TitleScreenSettingsMenu.h"

#include <SDL.h>
#include <Rocket/Core.h>

#include "ResourceLoader.h"
#include "GraphicsUtil.h"
#include "Scheduler.h"
#include "Settings.h"

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
      Rocket::Core::Element* optionsForm = titleSettingsDocument->GetElementById("optionsForm");
      
      if(optionsForm != NULL)
      {
         musicEnabledCheckbox = optionsForm->GetElementById("musicEnabled");
         if(musicEnabledCheckbox != NULL)
         {
            bindings.bindAction(musicEnabledCheckbox, "change", &TitleScreenSettingsMenu::onMusicEnabledChange);
         }
         
         soundEnabledCheckbox = optionsForm->GetElementById("soundEnabled");
         if(soundEnabledCheckbox != NULL)
         {
            bindings.bindAction(soundEnabledCheckbox, "change", &TitleScreenSettingsMenu::onSoundEnabledChange);
         }
         
         bindings.bindAction(optionsForm, "submit", &TitleScreenSettingsMenu::onSubmit);

         loadSettings();
      }

      titleSettingsDocument->Show();
   }
}

TitleScreenSettingsMenu::~TitleScreenSettingsMenu()
{
   titleSettingsDocument->Close();
   titleSettingsDocument->RemoveReference();

   delete scheduler;
}

void TitleScreenSettingsMenu::loadSettings()
{
   if(musicEnabledCheckbox != NULL)
   {
      if(Settings::getCurrentSettings().isMusicEnabled())
      {
         musicEnabledCheckbox->SetAttribute("checked", "");
      }
      else
      {
         musicEnabledCheckbox->RemoveAttribute("checked");
      }
   }
   
   if(soundEnabledCheckbox != NULL)
   {
      if(Settings::getCurrentSettings().isSoundEnabled())
      {
         soundEnabledCheckbox->SetAttribute("checked", "");
      }
      else
      {
         soundEnabledCheckbox->RemoveAttribute("checked");
      }
   }
}

void TitleScreenSettingsMenu::revertSettings()
{
   Settings::getCurrentSettings().revertChanges();
}

bool TitleScreenSettingsMenu::getCheckboxValue(Rocket::Core::Event* event)
{
   return event->GetCurrentElement()->GetAttribute("checked") != NULL;
}

void TitleScreenSettingsMenu::onMusicEnabledChange(Rocket::Core::Event* event)
{
   Settings::getCurrentSettings().setMusicEnabled(TitleScreenSettingsMenu::getCheckboxValue(event));
}

void TitleScreenSettingsMenu::onSoundEnabledChange(Rocket::Core::Event* event)
{
   Settings::getCurrentSettings().setSoundEnabled(TitleScreenSettingsMenu::getCheckboxValue(event));
}

void TitleScreenSettingsMenu::onSubmit(Rocket::Core::Event* event)
{
   if (event->GetParameter<Rocket::Core::String>("submit", "cancel") == "apply")
   {
      bool settingsUpdateSuccess = true;
      if(GraphicsUtil::getInstance()->isVideoModeRefreshRequired())
      {
         std::string* errorMsg = NULL;
         settingsUpdateSuccess = GraphicsUtil::getInstance()->refreshVideoMode(errorMsg);
         if(!settingsUpdateSuccess)
         {
            DEBUG("Failed to refresh video mode:");
            if(errorMsg != NULL)
            {
               DEBUG("%s", errorMsg->c_str());
               delete errorMsg;
               errorMsg = NULL;
            }
            else
            {
               DEBUG("No error message supplied.");
            }
            
            Settings::getCurrentSettings().revertVideoChanges();
            if(!GraphicsUtil::getInstance()->refreshVideoMode(errorMsg))
            {
               if(errorMsg != NULL)
               {
                  DEBUG("%s", errorMsg->c_str());
                  delete errorMsg;
                  errorMsg = NULL;
               }
               
               T_T("Failed to revert video mode!");
            }
         }
      }

      if(settingsUpdateSuccess)
      {
         saveSettings();
         finished = true;
      }
   }
   else
   {
      revertSettings();
      finished = true;
   }
}

void TitleScreenSettingsMenu::saveSettings()
{
   Settings::getCurrentSettings().save();
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
