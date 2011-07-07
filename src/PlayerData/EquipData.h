#ifndef EQUIP_DATA_H
#define EQUIP_DATA_H

#include <string>
#include <vector>

namespace Json
{
   class Value;
};

class Item;

class EquipData
{
   class Slot
   {
      const Item* equipped;
      int numAcceptedTypes;
      int* acceptedTypes;
      bool enabled;

      public:
         Slot();
         void load(Json::Value& inputJson);
         void serialize(Json::Value& slotNode);
         ~Slot();
   };

   Slot head;
   Slot body;
   Slot primaryWeapon;
   Slot primaryOffhand;
   Slot secondaryWeapon;
   Slot secondaryOffhand;
   Slot garment;
   Slot feet;

   std::vector<Slot> accessories;
   
   public:
      EquipData();
      void load(Json::Value& equipmentNode);
      void serialize(Json::Value& outputJson);
      ~EquipData();
};

#endif
