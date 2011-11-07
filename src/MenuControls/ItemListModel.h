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
      void setItems(ItemList& newList);
      void clear();
      int getNumberOfElements();
      std::string getElementAt(int i);
      const Item* getItemAt(int i);
};

#endif
