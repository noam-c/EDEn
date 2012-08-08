/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "GameState.h"
#include <functional>
#include <vector>

namespace Rocket
{
   namespace Core
   {
      class Context;
      class ElementDocument;
      class Event;
   };
};

template<typename T> class RocketListener;

class Music;
class Sound;

/**
 * Provides the title screen functionality, including the game logo, backdrop, 
 * and options for the user.
 * This state is the first that the user must interact with.
 *
 * @author Noam Chitayat
 */
class MainMenu: public GameState
{
   /** Main menu music */
   Music* music;

   /** Sound for hovering over an option */
   Sound* reselectSound;

   /** Sound for picking an option */
   Sound* chooseSound;

   /** The Rocket context used to render the title screen */
   Rocket::Core::Context* rocketContext;

   /** The title screen RML document */
   Rocket::Core::ElementDocument* titleDocument;

   /**
    * Wait for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   std::vector<RocketListener<MainMenu>*> clickListeners;
   void bindAction(const char* id, const char* eventType, void (MainMenu::*function)(Rocket::Core::Event*), bool capture = false);

   //Actions for the list ops - see documentation in MainMenuActions.cpp
   void NewGameAction(Rocket::Core::Event* event);
   void MenuPrototypeAction(Rocket::Core::Event* event);
   void LoadGameAction(Rocket::Core::Event* event);
   void OptionsAction(Rocket::Core::Event* event);
   void AboutAction(Rocket::Core::Event* event);
   void QuitAction(Rocket::Core::Event* event);

   protected:
      /**
       * Waits a millisecond between draws (no rush on a title screen)
       */
      void draw();

      /**
       * Perform logic for the title screen.
       * NOTE: this method will spin infinitely until user input is received.
       * Since the screen is static, there is no need to refresh without input.
       *
       * @return true iff the title screen is not finished running (no quit event)
       */
      bool step();

   public:
      /**
       * Constructor.
       * Initializes the title screen widgets, font, image and sounds.
       *
       * @param executionStack The execution stack that the state belongs to.
       */
      MainMenu(ExecutionStack& executionStack);
   
      /**
       * When the state is activated, set modal focus to the listbox
       */
      void activate();
   
      /**
       * Destructor.
       */
      ~MainMenu();
};

#endif
