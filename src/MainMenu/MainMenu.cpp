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

      bindAction(titleDocument, "keydown", &MainMenu::listKeyDown);
   }
}

void MainMenu::bindAction(const char* id, const char* eventType, void (MainMenu::*function)(Rocket::Core::Event*), bool capture)
{
   Rocket::Core::Element* element = titleDocument->GetElementById(id);
   if(element != NULL)
   {
      bindAction(element, eventType, function);
   }
   else
   {
      DEBUG("Unable to bind %s event listener to element with id: %s", eventType, id);
   }
}

void MainMenu::bindAction(Rocket::Core::Element* element, const char* eventType, void (MainMenu::*function)(Rocket::Core::Event*), bool capture)
{
   RocketListener<MainMenu>* listener = new RocketListener<MainMenu>(std::bind1st(std::mem_fun(function), this));
   clickListeners.push_back(listener);
   element->AddEventListener(eventType, listener, capture);
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

   RocketSDLInputMapping::handleSDLEvent(rocketContext, event);
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
   /* Don't run too fast */
   SDL_Delay (1);

   rocketContext->Render();
}

MainMenu::~MainMenu()
{
   titleDocument->RemoveReference();
   for(std::vector<RocketListener<MainMenu>* >::iterator iter = clickListeners.begin(); iter != clickListeners.end(); ++iter)
   {
      delete *iter;
   }

   rocketContext->RemoveReference();
}
