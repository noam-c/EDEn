#ifndef ITEMS_MENU_H
#define ITEMS_MENU_H

#include "MenuState.h"

class PlayerData;

class ItemsMenu : public MenuState
{
   PlayerData& playerData;

   public:
      ItemsMenu(MenuShell& menuShell, PlayerData& playerData);
      ~ItemsMenu();
};

#endif