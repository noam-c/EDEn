#include "EquipSlot.h"
#include "ItemData.h"
#include "Item.h"
#include "json.h"

EquipSlot::EquipSlot() : equipped(NULL), numAcceptedTypes(0), acceptedTypes(NULL), enabled(true)
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

void EquipSlot::serialize(Json::Value& slotNode)
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

EquipSlot::~EquipSlot()
{
   if(acceptedTypes != NULL)
   {
      delete [] acceptedTypes;
   }
}