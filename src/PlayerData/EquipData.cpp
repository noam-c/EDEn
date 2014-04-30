/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "EquipData.h"
#include "json.h"
#include "Metadata.h"
#include "Item.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

void EquipData::load(const Metadata& metadata, const Json::Value& equipment)
{
   if(equipment.isNull())
   {
      return;
   }

   m_head.load(metadata, equipment["Head"]);
   m_body.load(metadata, equipment["Body"]);
   m_primaryWeapon.load(metadata, equipment["Wpn1"]);
   m_primaryOffhand.load(metadata, equipment["Off1"]);
   m_secondaryWeapon.load(metadata, equipment["Wpn2"]);
   m_secondaryOffhand.load(metadata, equipment["Off2"]);
   m_garment.load(metadata, equipment["Garment"]);
   m_feet.load(metadata, equipment["Feet"]);

   const Json::Value& accessoriesNode = equipment["Accessories"];
   int numAccessories = accessoriesNode.size();

   m_accessories.resize(numAccessories);
   for(int i = 0; i < numAccessories; ++i)
   {
      m_accessories[i].load(metadata, accessoriesNode[i]);
   }
}

void EquipData::serialize(Json::Value& outputJson) const
{
   m_head.serialize(outputJson["Head"]);
   m_body.serialize(outputJson["Body"]);
   m_primaryWeapon.serialize(outputJson["Wpn1"]);
   m_primaryOffhand.serialize(outputJson["Off1"]);
   m_secondaryWeapon.serialize(outputJson["Wpn2"]);
   m_secondaryOffhand.serialize(outputJson["Off2"]);
   m_garment.serialize(outputJson["Garment"]);
   m_feet.serialize(outputJson["Feet"]);
   
   int numAccessories = m_accessories.size();
   
   Json::Value accessoriesNode(Json::arrayValue);
   accessoriesNode.resize(numAccessories);
   for(int i = 0; i < numAccessories; ++i)
   {
      m_accessories[i].serialize(accessoriesNode[i]);
   }
}

std::vector<EquipSlot*> EquipData::getSlots()
{
   std::vector<EquipSlot*> slots;
   slots.push_back(&m_head);
   slots.push_back(&m_body);
   slots.push_back(&m_primaryWeapon);
   slots.push_back(&m_primaryOffhand);
   slots.push_back(&m_secondaryWeapon);
   slots.push_back(&m_secondaryOffhand);
   slots.push_back(&m_garment);
   slots.push_back(&m_feet);
   
   for(std::vector<EquipSlot>::iterator iter = m_accessories.begin(); iter != m_accessories.end(); ++iter)
   {
      slots.push_back(&(*iter));
   }
   
   return slots;
}
