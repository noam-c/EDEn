/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_H
#define ITEM_H

#include <string>

namespace Json
{
   class Value;
};

/**
 * Metadata for an item. Since currently, all the games items are non-customizable (no 'unique' items),
 * a template is sufficient for describing all the properties that an item will have. As a result,
 * Item is not an object representative of a single item found in the world; it is the information describing a given item.
 */
class Item
{
   /** The unique identifier of this item. */
   const int id;
   
   /** The name of this item. */
   const std::string name;

   public:
      /**
       * Constructor.
       *
       * @param itemNode The JSON node containing the item metadata to load.
       */
      Item(Json::Value& itemNode);
   
      /**
       * @return The unique identifier of this item.
       */
      const int getId() const;
      
      /**
       * @return The name of this item.
       */
      const std::string& getName() const;
   
      /**
       * Destructor.
       */
      ~Item();
};

#endif
