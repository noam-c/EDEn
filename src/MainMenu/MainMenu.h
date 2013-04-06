/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "GameState.h"
#include "EdenRocketBindings.h"

namespace Rocket
{
   namespace Core
   {
      class ElementDocument;
      class Event;
   };
};

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
   /** The event binding collection for this GUI */
   EdenRocketBindings<MainMenu> bindings;

   /** The coroutine scheduler for the main menu's GUI scripts */
   Scheduler* scheduler;

   /** Main menu music */
   Music* music;

   /** Sound for hovering over an option */
   Sound* reselectSound;

   /** Sound for picking an option */
   Sound* chooseSound;

   /** The title screen RML document */
   Rocket::Core::ElementDocument* titleDocument;

   /**
    * Poll and handle the next input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   /**
    * Callback for Rocket event when a key is pressed
    * while the list of menu options has focus.
    *
    * @param event The user input event.
    */
   void listKeyDown(Rocket::Core::Event* event);

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
      bool step(long timePassed);

      /**
       * @return the main menu's script scheduler
       */
      Scheduler* getScheduler() const;

   public:
      /**
       * Constructor.
       * Initializes the title screen widgets, font, image and sounds.
       *
       * @param gameContext The context containing the current player data and execution stack.
       */
      MainMenu(GameContext& gameContext);
   
      /**
       * Destructor.
       */
      ~MainMenu();
};

#endif
