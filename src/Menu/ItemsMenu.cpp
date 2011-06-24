#include "ItemsMenu.h"
#include "ItemsPane.h"
#include "PlayerData.h"
#include "MenuShell.h"

ItemsMenu::ItemsMenu(MenuShell& menuShell, PlayerData& playerData) : MenuState(menuShell), playerData(playerData)
{
   setMenuPane(new ItemsPane(playerData, menuShell.getDimension()));
}

ItemsMenu::~ItemsMenu()
{
}