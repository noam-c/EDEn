#include "ItemsMenu.h"
#include "ItemsPane.h"
#include "PlayerData.h"
#include "MenuShell.h"

ItemsMenu::ItemsMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData) : MenuState(executionStack, menuShell), playerData(playerData)
{
   setMenuPane(new ItemsPane(playerData, menuShell.getDimension()));
}

ItemsMenu::~ItemsMenu()
{
}