#include "EquipData.h"
#include "SaveGameItemNames.h"
#include "json.h"
#include "ItemData.h"
#include "Item.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

EquipData::EquipData()
{
}

void EquipData::load(Json::Value& equipment)
{
   if(equipment.isNull())
   {
      return;
   }

   head.load(equipment["Head"]);
   body.load(equipment["Body"]);
   primaryWeapon.load(equipment["Wpn1"]);
   primaryOffhand.load(equipment["Off1"]);
   secondaryWeapon.load(equipment["Wpn2"]);
   secondaryOffhand.load(equipment["Off2"]);
   garment.load(equipment["Garment"]);
   feet.load(equipment["Feet"]);

   Json::Value& accessoriesNode = equipment["Accessories"];
   int numAccessories = accessoriesNode.size();
   accessories.resize(numAccessories);

   for(int i = 0; i < numAccessories; ++i)
   {
      accessories[i].load(accessoriesNode[i]);
   }
}

void EquipData::serialize(Json::Value& outputJson)
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

EquipSlot& EquipData::getHeadEquip()
{
   return head;
}

