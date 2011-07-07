#include "EquipData.h"
#include "SaveGameItemNames.h"
#include "json.h"
#include "ItemData.h"
#include "Item.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_PLAYER;

EquipData::Slot::Slot() : equipped(NULL), numAcceptedTypes(0), acceptedTypes(NULL), enabled(true)
{
}

void EquipData::Slot::load(Json::Value& inputJson)
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
   numAcceptedTypes = acceptedTypesNode.size();
   if(numAcceptedTypes > 0)
   {
      acceptedTypes = new int[acceptedTypesNode.size()];
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

void EquipData::Slot::serialize(Json::Value& slotNode)
{
   if(equipped != NULL)
   {
      slotNode["equipped"] = equipped->getId();
   }
   
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

EquipData::Slot::~Slot()
{
   if(acceptedTypes != NULL)
   {
      delete [] acceptedTypes;
   }
}

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
