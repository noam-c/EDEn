/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
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
   EdenRocketBindings<ItemMenu> m_bindings;

   /** The view model that exposes the inventory to the GUI */
   ItemViewModel m_itemViewModel;

   /**
    * Initializes the item menu pane and populates the sidebar.
    */
   void initialize();
   
   void itemClicked(Rocket::Core::Event* event);
   void useItem(int rowIndex);
   void dragStarted(Rocket::Core::Event* event);

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       */
      ItemMenu(GameContext& gameContext);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param menuShell The shell for the menu.
       */
      ItemMenu(GameContext& gameContext, MenuShell* menuShell);

      /**
       * Destructor.
       */
      ~ItemMenu();
};

#endif
