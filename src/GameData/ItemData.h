#ifndef ITEM_DATA_H
#define ITEM_DATA_H

#include <map>
#include <string>

#include "Singleton.h"

class Item;

class ItemData : public Singleton<ItemData>
{
   std::map<int, Item const*> items;

   public:
      void initialize();   
      Item const* getItem(int key);
      void finish();
};

#endif
