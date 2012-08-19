/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEMS_MENU_H
#define ITEMS_MENU_H

#include "MenuState.h"
#include "ModuleSelectListener.h"
#include "ItemListModel.h"

class PlayerData;

/**
 * The items pane displays a list of items in the player's inventory, and allows the player to use
 * these items, if they are usable.
 *
 * @author Noam Chitayat
 */
class ItemsMenu : public MenuState, public edwt::ModuleSelectListener
{
   /** The player data that the menu interacts with. */
   PlayerData& playerData;
   
   /** The list of items in the player's inventory. */
   ItemListModel inventoryList;
   
   public:
      /**
       * Constructor.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param menuShell The shell to display the items menu GUI in.
       * @param playerData The player data to operate on.
       */
      ItemsMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData);

      /**
       * Signals that an item was selected in the data menu.
       *
       * @param index The index of the item in the inventory.
       */
      void moduleSelected(int index, const std::string& eventId);
   
      /**
       * Destructor.
       */
      ~ItemsMenu();
};

#endif
