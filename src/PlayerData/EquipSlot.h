/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef EQUIP_SLOT_H
#define EQUIP_SLOT_H

#include <string>
#include <vector>

namespace Json
{
   class Value;
};

class Metadata;
typedef class Usable Item;

/**
 * An equipment slot. Can have an item equipped in it, and can have restrictions on the
 * accepted equipment types.
 *
 * @author Noam Chitayat
 */
struct EquipSlot final
{
   /** The currently equipped item (nullptr if nothing is equipped). */
   const Item* equipped = nullptr;

   /** The list of accepted types */
   std::vector<int> acceptedTypes;

   /**
    * True iff the slot can be used. False if it is unusable due to another piece of equipment.
    * The best example would be the free hand slot, if the main weapon is 2-handed. You can't use the free hand
    * in that case.
    */
   bool enabled = true;

   /**
    * Constructor.
    */
   EquipSlot();

   /**
    * Load in slot information from a savegame.
    *
    * @param inputJson The JSON node containing the slot information.
    */
   void load(const Metadata& metadata, const Json::Value& inputJson);

   /**
    * Serialize the slot information into a JSON node.
    *
    * @param slotNode The JSON node into which the slot information should be saved.
    */
   void serialize(Json::Value& slotNode) const;
};

#endif
