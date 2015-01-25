/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "ItemViewModel.h"
#include "PlayerData.h"
#include "Metadata.h"
#include "Inventory.h"
#include "Item.h"

#include <sstream>
#include <string>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_MENU

const Rocket::Core::String ItemViewModel::UnknownItemIconPath("data/images/icons/I_Rock01.png");

ItemViewModel::ItemViewModel(const Metadata& metadata, PlayerData& playerData) :
   Rocket::Controls::DataSource("itemViewModel"),
   m_metadata(metadata),
   m_playerData(playerData)
{
}

UsableId ItemViewModel::getItemId(int rowIndex) const
{
   const ItemList& itemList = m_playerData.getInventory()->getItemList();
   return itemList[rowIndex].first;
}

void ItemViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   if (table == "items")
   {
      const ItemList& itemList = m_playerData.getInventory()->getItemList();
      for (int i = 0; i < columns.size(); ++i)
      {
         const UsableId usableId = itemList[row_index].first;
         const int itemQuantity = itemList[row_index].second;
         const Item* rowItem = m_metadata.getItem(usableId);
         if (columns[i] == "name")
         {
            if(rowItem == nullptr)
            {
               row.emplace_back(13, "Unknown %d", usableId);
            }
            else
            {
               row.emplace_back(rowItem->getName().c_str());
            }
         }
         else if (columns[i] == "quantity")
         {
            row.emplace_back(5, "%d", itemQuantity);
         }
         else if (columns[i] == "icon")
         {
            if(rowItem == nullptr)
            {
               row.push_back(ItemViewModel::UnknownItemIconPath);
            }
            else
            {
               row.emplace_back(rowItem->getIconPath().c_str());
            }
         }
      }
   }
}

int ItemViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "items")
   {
      return m_playerData.getInventory()->getItemList().size();
   }

   return 0;
}

void ItemViewModel::refresh(int rowIndex)
{
   NotifyRowChange("items", rowIndex, 1);
}
