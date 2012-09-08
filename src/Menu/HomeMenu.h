/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef HOME_MENU_H
#define HOME_MENU_H

#include <stack>
#include <map>

#include "GameState.h"
#include "EdenRocketBindings.h"
#include "MenuShell.h"
#include "HomeViewModel.h"

namespace Rocket
{
   namespace Core
   {
      class Context;
      class ElementDocument;
      class Element;
      class Event;
   };
};

class PlayerData;
class Sound;

/**
 * The home menu is the first menu state of the menu.
 * It serves to summarize important information for the player and allows the player to select
 * a character to perform further actions on.
 *
 * @author Noam Chitayat
 */
class HomeMenu: public GameState
{
   /** The event binding collection for this GUI */
   EdenRocketBindings<HomeMenu> bindings;

   /** The shell that holds the common menu controls */
   MenuShell menuShell;

   /** The player data */
   PlayerData& playerData;

   /** The view model */
   HomeViewModel homeViewModel;

   /** The home menu RML document */
   Rocket::Core::ElementDocument* homePaneDocument;

   /**
    * Wait for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   protected:
      /**
       * Draws the menu.
       */
      void draw();

      /**
       * Perform logic for the menu screen.
       *
       * @return true iff the user is not finished with the menu.
       */
      bool step();

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param playerData The player data that the menu will display.
       */
      HomeMenu(ExecutionStack& executionStack, PlayerData& playerData);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param playerData The player data that the menu will display.
       * @param menuShell The shell for the menu
       */
      HomeMenu(ExecutionStack& executionStack, PlayerData& playerData, MenuShell& menuShell);

      void initialize();

      /**
       * Destructor.
       */
      ~HomeMenu();
};

#endif
