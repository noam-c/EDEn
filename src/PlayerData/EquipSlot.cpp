/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "EquipSlot.h"
#include "ItemData.h"
#include "Item.h"
#include "json.h"

EquipSlot::EquipSlot() :
   equipped(NULL),
   acceptedTypes(NULL),
   enabled(true)
{
}

void EquipSlot::load(Json::Value& inputJson)
{
   if(inputJson.isNull())
   {
      return;
   }
   
   Json::Value& equippedIdNode = inputJson["equipped"];
   if(equippedIdNode.isInt())
   {
      equipped = ItemData::getInstance()->getItem(equippedIdNode.asInt());
   }
   
   Json::Value& acceptedTypesNode = inputJson["types"];
   int numAcceptedTypes = acceptedTypesNode.size();
   if(numAcceptedTypes > 0)
   {
      acceptedTypes.resize(numAcceptedTypes);
      for(int i = 0; i < numAcceptedTypes; ++i)
      {
         acceptedTypes[i] = acceptedTypesNode[i].asInt();
      }
   }
   
   Json::Value& enabledNode = inputJson["enabled"];
   if(enabledNode.isBool())
   {
      enabled = enabledNode.asBool();
   }
}

void EquipSlot::serialize(Json::Value& slotNode) const
{
   if(equipped != NULL)
   {
      slotNode["equipped"] = equipped->getId();
   }
   
   int numAcceptedTypes = acceptedTypes.size();
   if(numAcceptedTypes > 0)
   {
      Json::Value acceptedTypesNode(Json::arrayValue);
      acceptedTypesNode.resize(numAcceptedTypes);
      for(int i = 0; i < numAcceptedTypes; ++i)
      {
         acceptedTypesNode[i] = acceptedTypes[i];
      }
      
      slotNode["types"] = acceptedTypesNode;
   }
   
   if(!enabled)
   {
      slotNode["enabled"] = false;
   }
}

EquipSlot::~EquipSlot()
{
}
