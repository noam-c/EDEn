#ifndef ITEMS_MENU_H
#define ITEMS_MENU_H

#include "MenuState.h"

class ItemsPane;
class PlayerData;

class ItemsMenu : public MenuState
{
   ItemsPane* itemsPane;
   PlayerData& playerData;

   public:
      ItemsMenu(edwt::Container* top, edwt::Container& menuPane, PlayerData& playerData);
      ~ItemsMenu();
};

#endif