#include "ItemsPane.h"
#include "ItemData.h"
#include "Item.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "ItemListModel.h"
#include <string>
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;


ItemsPane::ItemsPane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
   itemsList = new ItemListModel();

   listBox = new edwt::ListBox(itemsList);
   listBox->setNumColumns(2);
   listBox->setMinColumnWidth(0, 100);
   listBox->setMinColumnWidth(1, 20);
   listBox->setColumnAlignment(0, edwt::LEFT);
   listBox->setColumnAlignment(1, edwt::RIGHT);

   add(listBox);
   
   refresh();
}

void ItemsPane::refresh()
{
   ItemList inventory = playerData.getInventory();
   itemsList->setItems(inventory);
   
   listBox->adjustSize();
   listBox->adjustWidth();
}

ItemsPane::~ItemsPane()
{
   delete listBox;
   delete itemsList;
}
