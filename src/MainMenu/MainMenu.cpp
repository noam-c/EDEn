/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MainMenu.h"
#include "GraphicsUtil.h"

#include "TileEngine.h"

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "Container.h"
#include "Label.h"
#include "OpenGLTTF.h"
#include "StringListModel.h"
#include "ListBox.h"

#include <Rocket/Core.h>
#include "RocketSDLInputMapping.h"

#include "RocketListener.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
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

MainMenu::MainMenu(ExecutionStack& executionStack) : GameState(executionStack)
{
   chooseSound = ResourceLoader::getSound("choose");
   reselectSound = ResourceLoader::getSound("reselect");

   music = ResourceLoader::getMusic("title.mp3");

   rocketContext = Rocket::Core::CreateContext("title", Rocket::Core::Vector2i(GraphicsUtil::getInstance()->getWidth(), GraphicsUtil::getInstance()->getHeight()));
   titleDocument = rocketContext->LoadDocument("data/gui/title.rml");

   if(titleDocument != NULL)
   {
      titleDocument->Show();
      bindAction("newGameAction", "click", &MainMenu::NewGameAction);
      bindAction("menuPrototypeAction", "click", &MainMenu::MenuPrototypeAction);
      bindAction("loadGameAction", "click", &MainMenu::LoadGameAction);
      bindAction("optionsAction", "click", &MainMenu::OptionsAction);
      bindAction("aboutAction", "click", &MainMenu::AboutAction);
      bindAction("quitGameAction", "click", &MainMenu::QuitAction);
   }
}

void MainMenu::bindAction(const char* id, const char* eventType, void (MainMenu::*function)(Rocket::Core::Event*), bool capture)
{
   Rocket::Core::Element* element = titleDocument->GetElementById(id);
   if(element != NULL)
   {
      RocketListener<MainMenu>* listener = new RocketListener<MainMenu>(std::bind1st(std::mem_fun(function), this));
      clickListeners.push_back(listener);
      element->AddEventListener(eventType, listener, capture);
   }
   else
   {
      DEBUG("Unable to bind %s event listener to element with id: %s", eventType, id);
   }
}

void MainMenu::activate()
{
   GameState::activate();
}

bool MainMenu::step()
{
   if(finished) return false;

   bool done = false;

   if (!MUSIC_OFF)
   {
      music->play();
   }

   waitForInputEvent(done);

   rocketContext->Update();

   return !done;
}

void MainMenu::waitForInputEvent(bool& finishState)
{
   SDL_Event event;

   /* Check for events */
   SDL_WaitEvent(&event);

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

   RocketSDLInputMapping::handleSDLEvent(rocketContext, event);
}

void MainMenu::draw()
{
   /* Don't run too fast */
   SDL_Delay (1);

   rocketContext->Render();
}

MainMenu::~MainMenu()
{
   if (!MUSIC_OFF)
   {
      Music::fadeOutMusic(1000);
   }

   delete titleDocument;
   for(std::vector<RocketListener<MainMenu>* >::iterator iter = clickListeners.begin(); iter != clickListeners.end(); ++iter)
   {
      delete *iter;
   }

   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);

   rocketContext->RemoveReference();
}
