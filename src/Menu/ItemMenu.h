/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_MENU_H
#define ITEM_MENU_H

#include "MenuState.h"
#include "EdenRocketBindings.h"
#include "ItemViewModel.h"

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
class ItemMenu: public MenuState
{
   /** The event binding collection for this GUI */
   EdenRocketBindings<ItemMenu> bindings;

   /** The player data */
   PlayerData& playerData;

   /** The view model that exposes the inventory to the GUI */
   ItemViewModel itemViewModel;

   /**
    * Initializes the item menu pane and populates the sidebar.
    */
   void initialize();

   protected:
      /**
       * Activates the data menu and reveals the data pane.
       */
      void activate();

      /**
       * Deactivates the data menu and hides the data pane.
       */
      void deactivate();

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param playerData The player data that the menu will display.
       */
      ItemMenu(GameContext& gameContext, PlayerData& playerData);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param playerData The player data that the menu will display.
       * @param menuShell The shell for the menu.
       */
      ItemMenu(GameContext& gameContext, PlayerData& playerData, MenuShell* menuShell);

      /**
       * Destructor.
       */
      ~ItemMenu();

      void itemClicked(Rocket::Core::Event* event);
};

#endif