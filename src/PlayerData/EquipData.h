/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef EQUIP_DATA_H
#define EQUIP_DATA_H

#include <string>
#include <vector>
#include "EquipSlot.h"

namespace Json
{
   class Value;
};

class Item;

/**
 * A data structure used for representing the equipment of a character, in terms of the equipment slots
 * and their associated items.
 *
 * @author Noam Chitayat
 */
class EquipData
{
   /** The equipment slot for headgear. */
   EquipSlot head;

   /** The equipment slot for armour. */
   EquipSlot body;

   /** The equipment slot for the default weapon. */
   EquipSlot primaryWeapon;

   /** The equipment slot for the default item held in the non-weapon hand (i.e. shields). */
   EquipSlot primaryOffhand;

   /** The equipment slot for the secondary weapon. */
   EquipSlot secondaryWeapon;

   /** The equipment slot for the secondary item held in the non-weapon hand (i.e. shields). */
   EquipSlot secondaryOffhand;

   /** The equipment slot for capes and overgarments. */
   EquipSlot garment;

   /** The equipment slot for footgear. */
   EquipSlot feet;

   /** The accessories equipped by the character. Characters can equip multiple accessories. */
   std::vector<EquipSlot> accessories;
   
   public:
      /**
       * Constructor for an empty set of equipment.
       */
      EquipData();

      /**
       * Load equipment and slot data for this equipment set.
       *
       * @param equipmentNode The JSON node containing the data for this equipment set.
       */
      void load(Json::Value& equipmentNode);

      /**
       * Serialize the equipment set into the given node.
       *
       * @param outputJson The JSON node to serialize the equipment into.
       */
      void serialize(Json::Value& outputJson);
   
      /**
       * Destructor.
       */
      ~EquipData();

      /**
       * @return The list of equipment slots on the character.
       */
      std::vector<EquipSlot*> getSlots();
};

#endif
