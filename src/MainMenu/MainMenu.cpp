/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "MainMenu.h"

#include <SDL.h>
#include <RmlUi/Core.h>

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_TITLE

enum class MainMenuActions
{
   NEW_GAME_ACTION,
   LOAD_GAME_ACTION,
   OPTIONS_ACTION,
   ABOUT_ACTION,
   QUIT_GAME_ACTION,
   MENU_PROTOTYPE_ACTION,
};

MainMenu::MainMenu(GameContext& gameContext) :
   GameState(gameContext, GameStateType::TITLE, "MainMenu")
{
   m_chooseSound = ResourceLoader::getSound("choose");
   m_reselectSound = ResourceLoader::getSound("reselect");

   m_music = ResourceLoader::getMusic("title.mp3");

   m_titleDocument = m_rocketContext->LoadDocument("data/gui/title.rml");

   if(m_titleDocument != nullptr)
   {
      m_titleDocument->Show();
      m_bindings.bindAction(m_titleDocument, "newGameAction", "click", [this](Rml::Core::Event& event) { NewGameAction(); });
      m_bindings.bindAction(m_titleDocument, "menuPrototypeAction", "click", [this](Rml::Core::Event& event) { MenuPrototypeAction(); });
      m_bindings.bindAction(m_titleDocument, "battlePrototypeAction", "click", [this](Rml::Core::Event& event) { BattlePrototypeAction(); });
      m_bindings.bindAction(m_titleDocument, "loadGameAction", "click", [this](Rml::Core::Event& event) { LoadGameAction(); });
      m_bindings.bindAction(m_titleDocument, "optionsAction", "click", [this](Rml::Core::Event& event) { OptionsAction(); });
      m_bindings.bindAction(m_titleDocument, "aboutAction", "click", [this](Rml::Core::Event& event) { AboutAction(); });
      m_bindings.bindAction(m_titleDocument, "quitGameAction", "click", [this](Rml::Core::Event& event) { QuitAction(); });

      m_bindings.bindAction(m_titleDocument, "keydown", [this](Rml::Core::Event& event) { listKeyDown(event); });
   }
}

MainMenu::~MainMenu()
{
   m_titleDocument->Close();
}

bool MainMenu::step(long timePassed)
{
   if(m_finished) return false;

   m_scheduler.runCoroutines(timePassed);
   bool done = false;

   m_music->play();

   waitForInputEvent(done);

   return !done;
}

void MainMenu::waitForInputEvent(bool& finishState)
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
               QuitAction();
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
          QuitAction();
          return;
      }
      default:
      {
          break;
      }
   }

   handleEvent(event);
}

void MainMenu::listKeyDown(Rml::Core::Event& event)
{
   Rml::Core::Input::KeyIdentifier key = static_cast<Rml::Core::Input::KeyIdentifier>(event.GetParameter<int>("key_identifier", Rml::Core::Input::KI_UNKNOWN));

   switch(key)
   {
      case Rml::Core::Input::KI_UP:
      case Rml::Core::Input::KI_DOWN:
      case Rml::Core::Input::KI_RETURN:
         break;
      default:
         return;
   }

   auto list = m_titleDocument->GetElementById("menu");
   if(list == nullptr)
   {
      return;
   }

   auto child = list->GetFirstChild();
   while(child != nullptr)
   {
      if(child->IsClassSet("selected"))
      {
         break;
      }

      child = child->GetNextSibling();
   }

   if(child == nullptr)
   {
      return;
   }

   if(key == Rml::Core::Input::KI_RETURN)
   {
      child->Click();
   }
   else if(key == Rml::Core::Input::KI_UP)
   {
      auto previousSibling = child->GetPreviousSibling();
      if(previousSibling != nullptr)
      {
         child->SetClass("selected", false /*activate*/);
         previousSibling->SetClass("selected", true /*activate*/);
      }
   }
   else if(key == Rml::Core::Input::KI_DOWN)
   {
      auto nextSibling = child->GetNextSibling();
      if(nextSibling != nullptr)
      {
         child->SetClass("selected", false /*activate*/);
         nextSibling->SetClass("selected", true /*activate*/);
      }
   }
}

void MainMenu::draw()
{
}

Scheduler* MainMenu::getScheduler()
{
   return &m_scheduler;
}
