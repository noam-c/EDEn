/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "EquipSlot.h"
#include "Metadata.h"
#include "Item.h"
#include "json.h"

EquipSlot::EquipSlot() = default;

void EquipSlot::load(const Metadata& metadata, const Json::Value& inputJson)
{
   if(inputJson.isNull())
   {
      return;
   }

   const Json::Value& equippedIdNode = inputJson["equipped"];
   if(equippedIdNode.isInt())
   {
      equipped = metadata.getItem(equippedIdNode.asInt());
   }

   const Json::Value& acceptedTypesNode = inputJson["types"];
   int numAcceptedTypes = acceptedTypesNode.size();
   if(numAcceptedTypes > 0)
   {
      acceptedTypes.resize(numAcceptedTypes);
      for(int i = 0; i < numAcceptedTypes; ++i)
      {
         acceptedTypes[i] = acceptedTypesNode[i].asInt();
      }
   }

   const Json::Value& enabledNode = inputJson["enabled"];
   if(enabledNode.isBool())
   {
      enabled = enabledNode.asBool();
   }
}

void EquipSlot::serialize(Json::Value& slotNode) const
{
   if(equipped != nullptr)
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
