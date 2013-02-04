/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <map>
#include <string>

class GameContext;

typedef class Usable Item;

/**
 * A global map holding all the item metadata (item IDs and the associated names, descriptions, etc.) for the game.
 *
 * @author Noam Chitayat
 */
class ItemData
{
   /** A map of all the items, mapped by Item ID. */
   std::map<int, Item*> items;

   public:
      /**
       * Constructor. Load up all the item metadata from items.edb.
       */
      ItemData(GameContext& gameContext);

      /**
       * Destructor. Clean up the item metadata map.
       */
      ~ItemData();

      /**
       * @param key The ID of the item metadata to be retrieved.
       *
       * @return The metadata for the item with the specified ID.
       */
      Item* getItem(int key) const;
};

#endif
