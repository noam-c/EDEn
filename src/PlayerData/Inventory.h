/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef INVENTORY_H
#define INVENTORY_H

#include "ItemList.h"

namespace Json
{
   class Value;
};

typedef class Usable Item;

/**
 * Keeps track of all the items held in the player's inventory,
 * and allows scripts and the game engine to query or update the
 * items inside the inventory and their quantities.
 *
 * @author Noam Chitayat
 */
class Inventory
{
   static const char* ITEM_ELEMENT;
   static const char* ITEM_NUM_ATTRIBUTE;
   static const char* ITEM_QUANTITY_ATTRIBUTE;

   /** The items held in the inventory. */
   ItemList m_items;

   /**
    * Searches the inventory for the item quantity of the specified item ID.
    *
    * @param itemId The item ID to search for.
    * @return An iterator to the requested item entry, or items.end() if no item was found.
    */
   ItemList::const_iterator findItem(UsableId itemId) const;

   /**
    * Searches the inventory for the item quantity of the specified item ID.
    *
    * @param itemId The item ID to search for.
    * @return An iterator to the requested item entry, or items.end() if no item was found.
    */
   ItemList::iterator findItem(int itemId);

   public:
      static const char* INVENTORY_ELEMENT;

      /**
       * Assignment operator.
       */
      Inventory& operator=(const Inventory& inventory);

      /**
       * @return The item list that represents the inventory.
       * \todo Remove this function when possible to promote encapsulation.
       */
      const ItemList& getItemList() const;

      /**
       * Gets a list of items and their quantities in the inventory, filtered by a list of types.
       *
       * @param acceptedTypes The item types to search for.
       *
       * @return An item list containing only the items of the specified types, and their quantities in the inventory.
       */
      ItemList getItemsByTypes(std::vector<int> acceptedTypes) const;

      /**
       * Deserializes JSON data into the Inventory.
       *
       * @param inventoryJson The JSON data to load from.
       */
      void load(const Json::Value& inventoryJson);

      /**
       * Serializes the inventory into JSON.
       *
       * @return The serialized JSON for the inventory.
       */
      Json::Value serialize() const;

      /**
       * @param itemId The kind of item to count.
       *
       * @return The amount of the specified item in the inventory.
       */
      int getItemQuantity(int itemId) const;

      /**
       * @param itemId The kind of item to add to the inventory.
       * @param quantity The amount of the item to add to the inventory.
       *
       * @return True iff the item quantity update was successful.
       */
      bool addItem(int itemId, int quantity = 1);

      /**
       * @param itemId The kind of item to remove from the inventory.
       * @param quantity The amount of the item to remove from the inventory.
       *
       * @return True iff the item quantity update was successful.
       */
      bool removeItem(int itemId, int quantity = 1);
};

#endif
