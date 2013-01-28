/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Inventory.h"
#include "Item.h"
#include "json.h"

const char* Inventory::INVENTORY_ELEMENT = "Inventory";
const char* Inventory::ITEM_ELEMENT = "Item";
const char* Inventory::ITEM_NUM_ATTRIBUTE = "itemNum";
const char* Inventory::ITEM_QUANTITY_ATTRIBUTE = "quantity";

Inventory& Inventory::operator=(const Inventory& inventory)
{
   if(&inventory != this)
   {
      items = inventory.items;
   }

   return *this;
}

const ItemList& Inventory::getItemList() const
{
   return items;
}

ItemList Inventory::getItemsByTypes(std::vector<int> acceptedTypes) const
{
   /**
    * \todo This method needs to properly filter through the inventory by type,
    * This must be done once item type is loaded into the Item structure.
    */
   return items;
}

void Inventory::load(const Json::Value& inventoryJson)
{
   items.clear();

   for(Json::Value::iterator iter = inventoryJson.begin(); iter != inventoryJson.end(); ++iter)
   {
      int itemNum, itemQuantity;
      itemNum = (*iter)[Inventory::ITEM_NUM_ATTRIBUTE].asInt();
      itemQuantity = (*iter)[Inventory::ITEM_QUANTITY_ATTRIBUTE].asInt();
      items.push_back(std::make_pair(itemNum, itemQuantity));
   }
}

Json::Value Inventory::serialize() const
{
   Json::Value inventoryNode(Json::arrayValue);
   for(ItemList::const_iterator iter = items.begin(); iter != items.end(); ++iter)
   {
      int itemNumber = iter->first;
      int itemQuantity = iter->second;
      
      if(itemQuantity > 0)
      {
         Json::Value itemEntry(Json::objectValue);
         itemEntry[Inventory::ITEM_NUM_ATTRIBUTE] = itemNumber;
         itemEntry[Inventory::ITEM_QUANTITY_ATTRIBUTE] = itemQuantity;
         
         inventoryNode.append(itemEntry);
      }
   }
   
   return inventoryNode;
}

ItemList::const_iterator Inventory::findItem(int itemId) const
{
   ItemList::const_iterator itemIter;
   for(itemIter = items.begin(); itemIter != items.end(); ++itemIter)
   {
      if(itemIter->first == itemId)
      {
         break;
      }
   }

   return itemIter;
}

ItemList::iterator Inventory::findItem(int itemId)
{
   ItemList::iterator itemIter;
   for(itemIter = items.begin(); itemIter != items.end(); ++itemIter)
   {
      if(itemIter->first == itemId)
      {
         break;
      }
   }
   
   return itemIter;
}

int Inventory::getItemQuantity(int itemId) const
{
   ItemList::const_iterator itemIter = findItem(itemId);
   if(itemIter != items.end())
   {
      return itemIter->second;
   }

   return 0;
}

bool Inventory::addItem(int itemId, int quantity)
{
   if(quantity < 1)
   {
      return false;
   }
   
   ItemList::iterator itemIter = findItem(itemId);
   if(itemIter == items.end())
   {
      items.push_back(std::make_pair(itemId, quantity));
   }
   else
   {
      itemIter->second += quantity;
   }
   
   return true;
}

bool Inventory::removeItem(int itemId, int quantity)
{
   if(quantity < 1)
   {
      return false;
   }
   
   for(ItemList::iterator iter = items.begin(); iter != items.end(); ++iter)
   {
      int& existingQuantity = iter->second;
      if(iter->first == itemId)
      {
         if(existingQuantity >= quantity)
         {
            // If the item is in the inventory in sufficient quantity, remove 'quantity' items
            // and return true.
            existingQuantity -= quantity;
            if(existingQuantity == 0)
            {
               items.erase(iter);
            }
            
            return true;
         }
         
         break;
      }
   }
   
   // If the item is not in inventory, return false.
   return false;
}
