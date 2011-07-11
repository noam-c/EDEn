#ifndef EQUIP_DATA_H
#define EQUIP_DATA_H

#include <string>
#include <vector>

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
   /**
    * An equipment slot. Can have an item equipped in it, and can have restrictions on the 
    * accepted equipment types.
    *
    * @author Noam Chitayat
    */
   struct Slot
   {
      /** The currently equipped item (NULL if nothing is equipped). */
      const Item* equipped;

      /** The number of accepted types */
      int numAcceptedTypes;

      /** The list of accepted types */
      int* acceptedTypes;

      /** 
       * True iff the slot can be used. False if it is unusable due to another piece of equipment.
       * The best example would be the free hand slot, if the main weapon is 2-handed. You can't use the free hand
       * in that case.
       */
      bool enabled;

      /**
       * Constructor.
       */
      Slot();

      /**
       * Load in slot information from a savegame.
       *
       * @param inputJson The JSON node containing the slot information.
       */
      void load(Json::Value& inputJson);

      /**
       * Serialize the slot information into a JSON node.
       *
       * @param slotNode The JSON node into which the slot information should be saved.
       */
      void serialize(Json::Value& slotNode);
      
      /**
       * Destructor.
       */
      ~Slot();
   };

   /** The equipment slot for headgear. */
   Slot head;

   /** The equipment slot for armour. */
   Slot body;

   /** The equipment slot for the default weapon. */
   Slot primaryWeapon;

   /** The equipment slot for the default item held in the non-weapon hand (i.e. shields). */
   Slot primaryOffhand;

   /** The equipment slot for the secondary weapon. */
   Slot secondaryWeapon;

   /** The equipment slot for the secondary item held in the non-weapon hand (i.e. shields). */
   Slot secondaryOffhand;

   /** The equipment slot for capes and overgarments. */
   Slot garment;

   /** The equipment slot for footgear. */
   Slot feet;

   /** The accessories equipped by the character. Characters can equip multiple accessories. */
   std::vector<Slot> accessories;
   
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
       * @return The name of the item equipped to the head slot.
       */
      const std::string getHeadEquip() const;
};

#endif
