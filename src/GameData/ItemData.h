/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <map>
#include <string>

#include "Singleton.h"

class Item;

/**
 * A global map holding all the item metadata (item IDs and the associated names, descriptions, etc.) for the game.
 *
 * @author Noam Chitayat
 */
class ItemData : public Singleton<ItemData>
{
   /** A map of all the items, mapped by Item ID. */
   std::map<int, Item const*> items;

   public:
      /**
       * Load up all the item metadata from items.edb.
       */
      void initialize();

      /**
       * Get an item by its ID.
       *
       * @param key The ID of the item metadata to be retrieved.
       */
      Item const* getItem(int key);

      /**
       * Clean up the item metadata map.
       */
      void finish();
};

#endif
