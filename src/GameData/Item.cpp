/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Item.h"
#include "json.h"

Item::Item(Json::Value& node) :
   id(node["id"].asInt()),
   name(node["name"].asString())
{
}

Item::~Item()
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
