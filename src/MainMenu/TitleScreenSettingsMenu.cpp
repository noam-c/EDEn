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
   m_bindings(this)
{
   m_scheduler = new Scheduler();

   m_titleSettingsDocument = m_rocketContext->LoadDocument("data/gui/titleOptionsMenu.rml");

   if(m_titleSettingsDocument != NULL)
   {
      Rocket::Core::Element* optionsForm = m_titleSettingsDocument->GetElementById("optionsForm");
      
      if(optionsForm != NULL)
      {
         m_musicEnabledCheckbox = optionsForm->GetElementById("musicEnabled");
         if(m_musicEnabledCheckbox != NULL)
         {
            m_bindings.bindAction(m_musicEnabledCheckbox, "change", &TitleScreenSettingsMenu::onMusicEnabledChange);
         }
         
         m_soundEnabledCheckbox = optionsForm->GetElementById("soundEnabled");
         if(m_soundEnabledCheckbox != NULL)
         {
            m_bindings.bindAction(m_soundEnabledCheckbox, "change", &TitleScreenSettingsMenu::onSoundEnabledChange);
         }
         
         m_bindings.bindAction(optionsForm, "submit", &TitleScreenSettingsMenu::onSubmit);

         loadSettings();
      }

      m_titleSettingsDocument->Show();
   }
}

TitleScreenSettingsMenu::~TitleScreenSettingsMenu()
{
   m_titleSettingsDocument->Close();
   m_titleSettingsDocument->RemoveReference();

   delete m_scheduler;
}

void TitleScreenSettingsMenu::loadSettings()
{
   if(m_musicEnabledCheckbox != NULL)
   {
      if(Settings::getCurrentSettings().isMusicEnabled())
      {
         m_musicEnabledCheckbox->SetAttribute("checked", "");
      }
      else
      {
         m_musicEnabledCheckbox->RemoveAttribute("checked");
      }
   }
   
   if(m_soundEnabledCheckbox != NULL)
   {
      if(Settings::getCurrentSettings().isSoundEnabled())
      {
         m_soundEnabledCheckbox->SetAttribute("checked", "");
      }
      else
      {
         m_soundEnabledCheckbox->RemoveAttribute("checked");
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
   if(event->GetParameter<Rocket::Core::String>("submit", "cancel") == "apply")
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
         m_finished = true;
      }
   }
   else
   {
      revertSettings();
      m_finished = true;
   }
}

void TitleScreenSettingsMenu::saveSettings()
{
   Settings::getCurrentSettings().save();
}

bool TitleScreenSettingsMenu::step(long timePassed)
{
   if(m_finished)
   {
      return false;
   }

   m_scheduler->runCoroutines(timePassed);
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
               m_finished = true;
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
         m_finished = true;
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
   return m_scheduler;
}
