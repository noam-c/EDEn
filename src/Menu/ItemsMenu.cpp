#include "ItemsMenu.h"
#include "ItemsPane.h"
#include "PlayerData.h"

ItemsMenu::ItemsMenu(edwt::Container* top, edwt::Container& menuPane, PlayerData& playerData) : MenuState(top, menuPane), playerData(playerData)
{
   itemsPane = new ItemsPane(playerData, menuPane.getDimension());
   menuPane.add(itemsPane);
}

ItemsMenu::~ItemsMenu()
{
   menuPane.remove(itemsPane);
   delete itemsPane;
}