#include "ItemsMenu.h"
#include "ItemsPane.h"
#include "ItemListModel.h"
#include "Item.h"
#include "PlayerData.h"
#include "MenuShell.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

ItemsMenu::ItemsMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData) : MenuState(executionStack, menuShell), playerData(playerData)
{
   ItemList inventory = playerData.getInventory();
   inventoryList.setItems(inventory);
   ItemsPane* pane = new ItemsPane(inventoryList, menuShell.getDimension());
   pane->setModuleSelectListener(this);

   setMenuPane(pane);
}

void ItemsMenu::moduleSelected(int index, const std::string& eventId)
{
   const Item* item = inventoryList.getItemAt(index);
   DEBUG("Item selected: %s", item->getName().c_str());
}

ItemsMenu::~ItemsMenu()
{
}