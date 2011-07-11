#ifndef ITEMS_MENU_H
#define ITEMS_MENU_H

#include "MenuState.h"

class PlayerData;

/**
 * The items pane displays a list of items in the player's inventory, and allows the player to use
 * these items, if they are usable.
 *
 * @author Noam Chitayat
 */
class ItemsMenu : public MenuState
{
   /** The player data that the menu interacts with. */
   PlayerData& playerData;
   
   public:
      /**
       * Constructor.
       *
       * @param menuShell The shell to display the items menu GUI in.
       * @param playerData The player data to operate on.
       */
      ItemsMenu(MenuShell& menuShell, PlayerData& playerData);

      /**
       * Destructor.
       */
      ~ItemsMenu();
};

#endif