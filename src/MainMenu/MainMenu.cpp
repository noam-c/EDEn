/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "MainMenu.h"

#include <SDL.h>
#include <Rocket/Core.h>

#include "ResourceLoader.h"
#include "Scheduler.h"
#include "Music.h"
#include "Sound.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TITLE;

enum MainMenuActions
{
   NEW_GAME_ACTION,
   LOAD_GAME_ACTION,
   OPTIONS_ACTION,
   ABOUT_ACTION,
   QUIT_GAME_ACTION,
   MENU_PROTOTYPE_ACTION,
};

MainMenu::MainMenu(GameContext& gameContext) :
   GameState(gameContext, GameState::TITLE, "MainMenu"),
   m_bindings(this)
{
   m_scheduler = new Scheduler();

   m_chooseSound = ResourceLoader::getSound("choose");
   m_reselectSound = ResourceLoader::getSound("reselect");

   m_music = ResourceLoader::getMusic("title.mp3");

   m_titleDocument = m_rocketContext->LoadDocument("data/gui/title.rml");

   if(m_titleDocument != nullptr)
   {
      m_titleDocument->Show();
      m_bindings.bindAction(m_titleDocument, "newGameAction", "click", &MainMenu::NewGameAction);
      m_bindings.bindAction(m_titleDocument, "menuPrototypeAction", "click", &MainMenu::MenuPrototypeAction);
      m_bindings.bindAction(m_titleDocument, "loadGameAction", "click", &MainMenu::LoadGameAction);
      m_bindings.bindAction(m_titleDocument, "optionsAction", "click", &MainMenu::OptionsAction);
      m_bindings.bindAction(m_titleDocument, "aboutAction", "click", &MainMenu::AboutAction);
      m_bindings.bindAction(m_titleDocument, "quitGameAction", "click", &MainMenu::QuitAction);

      m_bindings.bindAction(m_titleDocument, "keydown", &MainMenu::listKeyDown);
   }
}

MainMenu::~MainMenu()
{
   m_titleDocument->Close();
   m_titleDocument->RemoveReference();

   delete m_scheduler;
}

bool MainMenu::step(long timePassed)
{
   if(m_finished) return false;

   m_scheduler->runCoroutines(timePassed);
   bool done = false;

   m_music->play();

   waitForInputEvent(done);

   /* The menu shouldn't run too fast */
   SDL_Delay (1);

   return !done;
}

void MainMenu::waitForInputEvent(bool& finishState)
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
               QuitAction(nullptr);
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
          QuitAction(nullptr);
          return;
      }
      default:
      {
          break;
      }
   }

   handleEvent(event);
}

void MainMenu::listKeyDown(Rocket::Core::Event* event)
{
   Rocket::Core::Input::KeyIdentifier key = static_cast<Rocket::Core::Input::KeyIdentifier>(event->GetParameter<int>("key_identifier", Rocket::Core::Input::KI_UNKNOWN));

   switch(key)
   {
      case Rocket::Core::Input::KI_UP:
      case Rocket::Core::Input::KI_DOWN:
      case Rocket::Core::Input::KI_RETURN:
         break;
      default:
         return;
   }

   Rocket::Core::Element* list = m_titleDocument->GetElementById("menu");
   if(list == nullptr)
   {
      return;
   }

   Rocket::Core::Element* child = list->GetFirstChild();
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

   if(key == Rocket::Core::Input::KI_RETURN)
   {
      child->Click();
   }
   else if(key == Rocket::Core::Input::KI_UP)
   {
      Rocket::Core::Element* previousSibling = child->GetPreviousSibling();
      if(previousSibling != nullptr)
      {
         child->SetClass("selected", false /*activate*/);
         previousSibling->SetClass("selected", true /*activate*/);
      }
   }
   else if(key == Rocket::Core::Input::KI_DOWN)
   {
      Rocket::Core::Element* nextSibling = child->GetNextSibling();
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

Scheduler* MainMenu::getScheduler() const
{
   return m_scheduler;
}
