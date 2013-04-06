/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef HOME_MENU_H
#define HOME_MENU_H

#include <stack>
#include <map>

#include "MenuState.h"
#include "EdenRocketBindings.h"
#include "HomeViewModel.h"

namespace Rocket
{
   namespace Core
   {
      class Context;
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
class HomeMenu: public MenuState
{
   /** The event binding collection for this GUI */
   EdenRocketBindings<HomeMenu> bindings;

   /** The player data */
   PlayerData& playerData;

   /** The view model */
   HomeViewModel homeViewModel;

   /**
    * Initializes the home menu pane and populates the sidebar.
    */
   void initialize();

   /**
    * @param event The click event that occurred in the party list.
    */
   void characterClicked(Rocket::Core::Event* event);

   protected:
      /**
       * Activates the home menu and reveals the home pane.
       */
      void activate();

      /**
       * Deactivates the home menu and hides the home pane.
       */
      void deactivate();

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param playerData The player data that the menu will display.
       */
      HomeMenu(GameContext& gameContext, PlayerData& playerData);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param playerData The player data that the menu will display.
       * @param menuShell The shell for the menu
       */
      HomeMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell);

      /**
       * Destructor.
       */
      ~HomeMenu();

      /**
       * Handles sidebar option click events by navigating to the
       * appropriate menu.
       *
       * @param optionIndex The index of the sidebar option that was clicked.
       */
      void sidebarClicked(int optionIndex);
};

#endif
