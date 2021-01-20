/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "TitleScreenSettingsMenu.h"

#include <SDL.h>
#include <RmlUi/Core.h>

#include "ResourceLoader.h"
#include "GraphicsUtil.h"
#include "Settings.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_SETTINGS | DEBUG_TITLE

TitleScreenSettingsMenu::TitleScreenSettingsMenu(GameContext& gameContext) :
   GameState(gameContext, GameStateType::TITLE, "TitleScreenSettingsMenu")
{
   m_titleSettingsDocument = m_rocketContext->LoadDocument("data/gui/titleOptionsMenu.rml");

   if(m_titleSettingsDocument != nullptr)
   {
      auto optionsForm = m_titleSettingsDocument->GetElementById("optionsForm");

      if(optionsForm != nullptr)
      {
         m_musicEnabledCheckbox = optionsForm->GetElementById("musicEnabled");
         if(m_musicEnabledCheckbox != nullptr)
         {
            m_bindings.bindAction(m_musicEnabledCheckbox, "change", [this](Rml::Core::Event& event) { onMusicEnabledChange(event); });
         }

         m_soundEnabledCheckbox = optionsForm->GetElementById("soundEnabled");
         if(m_soundEnabledCheckbox != nullptr)
         {
            m_bindings.bindAction(m_soundEnabledCheckbox, "change", [this](Rml::Core::Event& event) { onSoundEnabledChange(event); });
         }

         m_bindings.bindAction(optionsForm, "submit", [this](Rml::Core::Event& event) { onSubmit(event); });

         loadSettings();
      }

      m_titleSettingsDocument->Show();
   }
}

TitleScreenSettingsMenu::~TitleScreenSettingsMenu()
{
   m_titleSettingsDocument->Close();
}

void TitleScreenSettingsMenu::loadSettings()
{
   if(m_musicEnabledCheckbox != nullptr)
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

   if(m_soundEnabledCheckbox != nullptr)
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

bool TitleScreenSettingsMenu::getCheckboxValue(Rml::Core::Event& event)
{
   return event.GetCurrentElement()->GetAttribute("checked") != nullptr;
}

void TitleScreenSettingsMenu::onMusicEnabledChange(Rml::Core::Event& event)
{
   Settings::getCurrentSettings().setMusicEnabled(TitleScreenSettingsMenu::getCheckboxValue(event));
}

void TitleScreenSettingsMenu::onSoundEnabledChange(Rml::Core::Event& event)
{
   Settings::getCurrentSettings().setSoundEnabled(TitleScreenSettingsMenu::getCheckboxValue(event));
}

void TitleScreenSettingsMenu::onSubmit(Rml::Core::Event& event)
{
   if(event.GetParameter<Rml::Core::String>("submit", "cancel") == "apply")
   {
      bool settingsUpdateSuccess = true;
      if(GraphicsUtil::getInstance()->isVideoModeRefreshRequired())
      {
         auto settingsUpdateResult = GraphicsUtil::getInstance()->refreshVideoMode();

         settingsUpdateSuccess = std::get<0>(settingsUpdateResult);
         if(!settingsUpdateSuccess)
         {
            DEBUG("Failed to refresh video mode:");
            auto errorMsg = std::get<1>(settingsUpdateResult);
            if(!errorMsg.empty())
            {
               DEBUG("%s", errorMsg.c_str());
            }
            else
            {
               DEBUG("No error message supplied.");
            }

            Settings::getCurrentSettings().revertVideoChanges();
            settingsUpdateResult = GraphicsUtil::getInstance()->refreshVideoMode();

            settingsUpdateSuccess = std::get<0>(settingsUpdateResult);
            if(!settingsUpdateSuccess)
            {
               errorMsg = std::get<1>(settingsUpdateResult);
               if(!errorMsg.empty())
               {
                  DEBUG("%s", errorMsg.c_str());
               }

               T_T("Failed to revert video mode!");
            }
         }
      }

      if(settingsUpdateSuccess)
      {
         saveSettings();
      }
   }
   else
   {
      revertSettings();
   }

   m_finished = true;
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

   m_scheduler.runCoroutines(timePassed);
   bool done = false;

   waitForInputEvent(done);

   return !done;
}

void TitleScreenSettingsMenu::waitForInputEvent(bool& finishState)
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

Scheduler* TitleScreenSettingsMenu::getScheduler()
{
   return &m_scheduler;
}
