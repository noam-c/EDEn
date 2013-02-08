/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "EquipData.h"
#include "json.h"
#include "Metadata.h"
#include "Item.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

EquipData::EquipData()
{
}

void EquipData::load(const GameContext& gameContext, Json::Value& equipment)
{
   if(equipment.isNull())
   {
      return;
   }

   head.load(gameContext, equipment["Head"]);
   body.load(gameContext, equipment["Body"]);
   primaryWeapon.load(gameContext, equipment["Wpn1"]);
   primaryOffhand.load(gameContext, equipment["Off1"]);
   secondaryWeapon.load(gameContext, equipment["Wpn2"]);
   secondaryOffhand.load(gameContext, equipment["Off2"]);
   garment.load(gameContext, equipment["Garment"]);
   feet.load(gameContext, equipment["Feet"]);

   Json::Value& accessoriesNode = equipment["Accessories"];
   int numAccessories = accessoriesNode.size();

   accessories.resize(numAccessories);
   for(int i = 0; i < numAccessories; ++i)
   {
      accessories[i].load(gameContext, accessoriesNode[i]);
   }
}

void EquipData::serialize(Json::Value& outputJson) const
{
   head.serialize(outputJson["Head"]);
   body.serialize(outputJson["Body"]);
   primaryWeapon.serialize(outputJson["Wpn1"]);
   primaryOffhand.serialize(outputJson["Off1"]);
   secondaryWeapon.serialize(outputJson["Wpn2"]);
   secondaryOffhand.serialize(outputJson["Off2"]);
   garment.serialize(outputJson["Garment"]);
   feet.serialize(outputJson["Feet"]);
   
   int numAccessories = accessories.size();
   
   Json::Value accessoriesNode(Json::arrayValue);
   accessoriesNode.resize(numAccessories);
   for(int i = 0; i < numAccessories; ++i)
   {
      accessories[i].serialize(accessoriesNode[i]);
   }
}

EquipData::~EquipData()
{
}

std::vector<EquipSlot*> EquipData::getSlots()
{
   std::vector<EquipSlot*> slots;
   slots.push_back(&head);
   slots.push_back(&body);
   slots.push_back(&primaryWeapon);
   slots.push_back(&primaryOffhand);
   slots.push_back(&secondaryWeapon);
   slots.push_back(&secondaryOffhand);
   slots.push_back(&garment);
   slots.push_back(&feet);
   
   for(std::vector<EquipSlot>::iterator iter = accessories.begin(); iter != accessories.end(); ++iter)
   {
      slots.push_back(&(*iter));
   }
   
   return slots;
}

