/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
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

typedef class Usable Item;

/**
 * A data structure used for representing the equipment of a character, in terms of the equipment slots
 * and their associated items.
 *
 * @author Noam Chitayat
 */
class EquipData
{
   /** The equipment slot for headgear. */
   EquipSlot m_head;

   /** The equipment slot for armour. */
   EquipSlot m_body;

   /** The equipment slot for the default weapon. */
   EquipSlot m_primaryWeapon;

   /** The equipment slot for the default item held in the non-weapon hand (i.e. shields). */
   EquipSlot m_primaryOffhand;

   /** The equipment slot for the secondary weapon. */
   EquipSlot m_secondaryWeapon;

   /** The equipment slot for the secondary item held in the non-weapon hand (i.e. shields). */
   EquipSlot m_secondaryOffhand;

   /** The equipment slot for capes and overgarments. */
   EquipSlot m_garment;

   /** The equipment slot for footgear. */
   EquipSlot m_feet;

   /** The accessories equipped by the character. Characters can equip multiple accessories. */
   std::vector<EquipSlot> m_accessories;

   public:
      /**
       * Load equipment and slot data for this equipment set.
       *
       * @param equipmentNode The JSON node containing the data for this equipment set.
       */
      void load(const Metadata& metadata, const Json::Value& equipmentNode);

      /**
       * Serialize the equipment set into the given node.
       *
       * @param outputJson The JSON node to serialize the equipment into.
       */
      void serialize(Json::Value& outputJson) const;

      /**
       * @return The list of equipment slots on the character.
       * \todo Refactor the code so that this method can either be const or removed
       */
      std::vector<EquipSlot*> getSlots();
};

#endif
