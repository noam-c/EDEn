/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEM_LIST_MODEL_H
#define ITEM_LIST_MODEL_H

#include "ItemList.h"
#include "guichan.hpp"
#include <vector>

class Item;

class ItemListModel : public gcn::ListModel
{
   ItemList itemList;
   
   public:
      void setItems(const ItemList& newList);
      void clear();
      int getNumberOfElements();
      std::string getElementAt(int i);
      const Item* getItemAt(int i) const;
};

#endif
