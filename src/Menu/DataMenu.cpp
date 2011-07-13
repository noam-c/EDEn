#include "DataMenu.h"
#include "DataPane.h"
#include "PlayerData.h"
#include "MenuShell.h"

DataMenu::DataMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData) : MenuState(executionStack, menuShell), playerData(playerData)
{
   setMenuPane(new DataPane(playerData, menuShell.getDimension()));
}

DataMenu::~DataMenu()
{
}
