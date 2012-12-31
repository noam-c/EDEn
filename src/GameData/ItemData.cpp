/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ItemData.h"
#include "Item.h"
#include "json.h"
#include <fstream>

#include "DebugUtils.h"

const int debugFlag = DEBUG_PLAYER;
const char* ITEM_DATA_PATH = "data/metadata/items.edb";

ItemData::ItemData(GameContext& gameContext)
{
   Json::Reader reader;
   DEBUG("Loading item data file %s", ITEM_DATA_PATH);

   std::ifstream input(ITEM_DATA_PATH);
   if(!input)
   {
      T_T("Failed to open data file for reading.");
   }

   Json::Value jsonRoot;
   if(!reader.parse(input, jsonRoot))
   {
      DEBUG("Failed to parse item data: %s", reader.getFormattedErrorMessages().c_str());
      T_T("Item database is corrupt.");
   }

   Json::Value& itemList = jsonRoot["data"];

   if(!itemList.isArray())
   {
      T_T("Failed to parse item data.");
   }

   int numItems = itemList.size();
   for(int i = 0; i < numItems; ++i)
   {
      int id = itemList[i]["id"].asInt();
      items[id] = new Item(itemList[i]);
      DEBUG("Loaded item ID %d", id);
   } 

   DEBUG("Item data loaded.");
}

ItemData::~ItemData()
{
   for(std::map<int, Item*>::iterator iter = items.begin(); iter != items.end(); ++iter)
   {
      delete iter->second;
   }
}

Item* ItemData::getItem(int key) const
{
   std::map<int, Item*>::const_iterator itemIterator = items.find(key);
   if(itemIterator != items.end())
   {
      return itemIterator->second;
   }

   return NULL;
}
