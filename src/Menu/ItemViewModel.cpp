/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
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
   gameContext.getItem(itemList[rowIndex].first)->onMenuUse(gameContext);
   NotifyRowChange("saveGames", rowIndex, 1);
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
         if (columns[i] == "name")
         {
            row.push_back(gameContext.getItem(itemList[row_index].first)->getName().c_str());
         }
         else if (columns[i] == "quantity")
         {
            std::ostringstream stream;
            stream << itemList[row_index].second;
            row.push_back(stream.str().c_str());
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
