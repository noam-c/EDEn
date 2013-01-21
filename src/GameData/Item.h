/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_H
#define ITEM_H

#include <string>

namespace Json
{
   class Value;
};

class GameContext;
class ItemScript;

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

   /** The path to the icon use to represent this item visually. */
   const std::string iconPath;

   /** The script that dictates what the item does when used. */
   ItemScript* itemScript;

   public:
      /**
       * Constructor.
       *
       * @param itemNode The JSON node containing the item metadata to load.
       */
      Item(Json::Value& itemNode);
   
      /**
       * Destructor.
       */
      ~Item();

      /**
       * @return The unique identifier of this item.
       */
      const int getId() const;
      
      /**
       * @return The name of this item.
       */
      const std::string& getName() const;

      /**
       * @return The icon visually representing this item.
       */
      const std::string& getIconPath() const;

      void loadScript(GameContext& gameContext);

      bool onMenuUse(GameContext& gameContext);

      bool onFieldUse(GameContext& gameContext);

      bool onBattleUse(GameContext& gameContext);
};

#endif
