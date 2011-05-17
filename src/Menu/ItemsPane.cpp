#include "ItemsPane.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "StringListModel.h"
#include <string>

ItemsPane::ItemsPane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
   itemsList = new edwt::StringListModel();

   listBox = new edwt::ListBox(itemsList);
   listBox->setDimension(rect);
   add(listBox);
   
   refresh();
}

std::string getItemName(int itemNum)
{
   return "Potion";
}

void ItemsPane::refresh()
{
   itemsList->clear();
   ItemList inventory = playerData.getInventory();
   
   for(ItemList::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)
   {
      itemsList->add(getItemName(iter->first), iter->first);
   }

   listBox->adjustSize();
   listBox->adjustWidth();
}

ItemsPane::~ItemsPane()
{
   delete listBox;
   delete itemsList;
}