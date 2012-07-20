/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ItemListModel.h"
#include "ItemData.h"
#include "Item.h"
#include <sstream>

void ItemListModel::setItems(const ItemList& newList)
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
   std::stringstream stream;
   stream << getItemAt(i)->getName();
   stream << '\t';
   stream << itemList[i].second;

   return stream.str();
}

const Item* ItemListModel::getItemAt(int i) const
{
   return ItemData::getInstance()->getItem(itemList[i].first);
}
