#include "Item.h"
#include "json.h"

Item::Item(Json::Value& node) : id(node["id"].asInt()), name(node["name"].asString())
{
}

const int Item::getId() const
{
   return id;
}

const std::string& Item::getName() const
{
   return name;
}

Item::~Item()
{
}
