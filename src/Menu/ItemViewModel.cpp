/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ItemViewModel.h"
#include "PlayerData.h"
#include "GameContext.h"
#include "Inventory.h"
#include "Item.h"

#include "dirent.h"
#include <sstream>
#include <string>

#include "DebugUtils.h"
const int debugFlag = DEBUG_MENU;

const Rocket::Core::String ItemViewModel::UnknownItemIconPath("data/images/icons/I_Rock01.png");

ItemViewModel::ItemViewModel(GameContext& gameContext, PlayerData& playerData) :
      Rocket::Controls::DataSource("itemViewModel"),
      gameContext(gameContext),
      playerData(playerData)
{
}

ItemViewModel::~ItemViewModel()
{
}

void ItemViewModel::useItem(int rowIndex)
{
   const ItemList& itemList = playerData.getInventory()->getItemList();
   const UsableId usableId = itemList[rowIndex].first;
   Item* item = gameContext.getItem(usableId);
   if(item == NULL)
   {
      DEBUG("Tried to use bad item with ID: %d.", usableId);
   }
   else
   {
      item->use(gameContext);
      NotifyRowChange("items", rowIndex, 1);
   }
}

UsableId ItemViewModel::getItemId(int rowIndex) const
{
   const ItemList& itemList = playerData.getInventory()->getItemList();
   return itemList[rowIndex].first;
}

void ItemViewModel::GetRow(Rocket::Core::StringList& row,
      const Rocket::Core::String& table, int row_index,
      const Rocket::Core::StringList& columns)
{
   if (table == "items")
   {
      const ItemList& itemList = playerData.getInventory()->getItemList();
      for (int i = 0; i < columns.size(); ++i)
      {
         const UsableId usableId = itemList[row_index].first;
         const int itemQuantity = itemList[row_index].second;
         const Item* rowItem = gameContext.getItem(usableId);
         if (columns[i] == "name")
         {
            if(rowItem == NULL)
            {
               row.push_back(Rocket::Core::String(13, "Unknown %d", usableId));
            }
            else
            {
               row.push_back(rowItem->getName().c_str());
            }
         }
         else if (columns[i] == "quantity")
         {
            row.push_back(Rocket::Core::String(5, "%d", itemQuantity));
         }
         else if (columns[i] == "icon")
         {
            if(rowItem == NULL)
            {
               row.push_back(ItemViewModel::UnknownItemIconPath);
            }
            else
            {
               row.push_back(rowItem->getIconPath().c_str());
            }
         }
      }
   }
}

int ItemViewModel::GetNumRows(const Rocket::Core::String& table)
{
   if (table == "items")
   {
      return playerData.getInventory()->getItemList().size();
   }

   return 0;
}
