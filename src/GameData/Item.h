#ifndef ITEM_H
#define ITEM_H

#include <string>

namespace Json
{
   class Value;
};

class Item
{
   const int id;
   const std::string name;

   public:
      Item(Json::Value& name);
      const int getId() const;
      const std::string& getName() const;
      ~Item();
};

#endif
