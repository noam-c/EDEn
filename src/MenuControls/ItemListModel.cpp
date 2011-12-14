/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "ItemListModel.h"
#include "ItemData.h"
#include "Item.h"
#include <stdio.h>

void ItemListModel::setItems(ItemList& newList)
{
   itemList.assign(newList.begin(), newList.end());
}

void ItemListModel::clear()
{
   itemList.clear();
}

int ItemListModel::getNumberOfElements()
{
   return itemList.size();
}

std::string ItemListModel::getElementAt(int i)
{
   std::string itemName = getItemAt(i)->getName();
   int itemQuantity = itemList[i].second;

   char buffer[itemName.length() + 40];
   return std::string(buffer, sprintf(buffer, "%s\t%d", itemName.c_str(), itemQuantity));
}

const Item* ItemListModel::getItemAt(int i)
{
   return ItemData::getInstance()->getItem(itemList[i].first);
}
