#include "ItemsPane.h"
#include "ItemData.h"
#include "Item.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "StringListModel.h"
#include <string>
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;


ItemsPane::ItemsPane(PlayerData& playerData, const gcn::Rectangle& rect) : MenuPane(rect), playerData(playerData)
{
   itemsList = new edwt::StringListModel();

   listBox = new edwt::ListBox(itemsList);
   listBox->setDimension(rect);
   add(listBox);
   
   refresh();
}

void ItemsPane::refresh()
{
   itemsList->clear();
   ItemList inventory = playerData.getInventory();
   
   for(ItemList::iterator iter = inventory.begin(); iter != inventory.end(); ++iter)
   {
      const std::string& itemName = ItemData::getInstance()->getItem(iter->first)->getName();
      DEBUG("Adding item '%s' to the menu...", itemName.c_str());
      itemsList->add(itemName, iter->first);
   }

   listBox->adjustSize();
   listBox->adjustWidth();
}

ItemsPane::~ItemsPane()
{
   delete listBox;
   delete itemsList;
}
