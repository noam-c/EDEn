/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
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
   GameState(gameContext, "MainMenu"),
   bindings(this)
{
   scheduler = new Scheduler();

   chooseSound = ResourceLoader::getSound("choose");
   reselectSound = ResourceLoader::getSound("reselect");

   music = ResourceLoader::getMusic("title.mp3");

   titleDocument = rocketContext->LoadDocument("data/gui/title.rml");

   if(titleDocument != NULL)
   {
      titleDocument->Show();
      bindings.bindAction(titleDocument, "newGameAction", "click", &MainMenu::NewGameAction);
      bindings.bindAction(titleDocument, "menuPrototypeAction", "click", &MainMenu::MenuPrototypeAction);
      bindings.bindAction(titleDocument, "loadGameAction", "click", &MainMenu::LoadGameAction);
      bindings.bindAction(titleDocument, "optionsAction", "click", &MainMenu::OptionsAction);
      bindings.bindAction(titleDocument, "aboutAction", "click", &MainMenu::AboutAction);
      bindings.bindAction(titleDocument, "quitGameAction", "click", &MainMenu::QuitAction);

      bindings.bindAction(titleDocument, "keydown", &MainMenu::listKeyDown);
   }
}

MainMenu::~MainMenu()
{
   titleDocument->Close();
   titleDocument->RemoveReference();

   delete scheduler;
}

bool MainMenu::step(long timePassed)
{
   if(finished) return false;

   scheduler->runCoroutines(timePassed);
   bool done = false;

   if (!MUSIC_OFF)
   {
      music->play();
   }

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
               QuitAction(NULL);
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
          QuitAction(NULL);
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

   Rocket::Core::Element* list = titleDocument->GetElementById("menu");
   if(list == NULL)
   {
      return;
   }

   Rocket::Core::Element* child = list->GetFirstChild();
   while(child != NULL)
   {
      if(child->IsClassSet("selected"))
      {
         break;
      }

      child = child->GetNextSibling();
   }

   if(key == Rocket::Core::Input::KI_RETURN)
   {
      child->Click();
   }
   else if(key == Rocket::Core::Input::KI_UP)
   {
      Rocket::Core::Element* previousSibling = child->GetPreviousSibling();
      if(previousSibling != NULL)
      {
         child->SetClass("selected", false /*activate*/);
         previousSibling->SetClass("selected", true /*activate*/);
      }
   }
   else if(key == Rocket::Core::Input::KI_DOWN)
   {
      Rocket::Core::Element* nextSibling = child->GetNextSibling();
      if(nextSibling != NULL)
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
   return scheduler;
}
