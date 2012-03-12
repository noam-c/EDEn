/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include "EquipData.h"

namespace Json
{
   class Value;
};

/**
 * A model holding the data for a character that can be used by the player.
 * All relevant attributes, skills and equipment should be held in this class.
 *
 * @author Noam Chitayat
 */
class Character
{
   /** The name of the character. */
   std::string name;

   /** The path to the character portrait (used in dialogue and menus). */
   std::string portraitPath;

   // Character status attributes
   int strength;
   int intelligence;
   int vitality;
   int reflex;
   int focus;
   int endurance;
   int agility;

   // Health and stamina of the character.
   int maxHP;
   int maxSP;
   int hp;
   int sp;
  
   /** The equipment worn by this Character. */
   EquipData equipment;
 
   /**
    * Parse the portrait path from the character node.
    *
    * @param charToLoad The JSON node containing the character data to load.
    */
   void parsePortraitData(Json::Value& charToLoad);
   
   /**
    * Serialize the portrait data into a JSON output.
    *
    * @param characterNode The JSON node representing the character to add the portrait data to.
    */
   void serializePortraitData(Json::Value& characterNode) const;
   
   public:
      /**
       * Constructor used to create a new character.
       *
       * @param The name of this character.
       */
      Character(const std::string& name);

      /**
       * Constructor used to load an existing character.
       *
       * @param charToLoad The JSON node containing the character data to load.
       */
      Character(Json::Value& charToLoad);
   
      /**
       * Serialize the character data into a JSON output.
       *
       * @param characterSet The character array to serialize the character into.
       */
      void serialize(Json::Value& characterSet) const;
   
      /**
       * @return The name of the character.
       */
      std::string getName() const;
   
      /**
       * @return The relative file path containing the picture of this character.
       * \todo The character portrait data should just contain an ID for the picture instead of a filepath.
       */
      std::string getPortraitPath() const;

      // Getters for the health and stamina.
      int getMaxHP() const;
      int getMaxSP() const;
      int getHP() const;
      int getSP() const;

      // Getters for the character attributes.
      int getStrength() const;
      int getIntelligence() const;
      int getVitality() const;
      int getReflex() const;
      int getFocus() const;
      int getEndurance() const;
      int getAgility() const;
      
      /**
       * @return The character's equipment information.
       */
      EquipData& getEquipment();
      
      /**
       * Sets a piece of equipment, and changes the character's stat bonuses accordingly.
       *
       * @param slot The equipment slot to equip.
       * @param newEquipment The new piece of equipment to put into the slot.
       */
      bool equip(EquipSlot& slot, const Item* newEquipment);
      
      /**
       * Removes a piece equipment, and changes the character's stat bonuses accordingly.
       *
       * @param slot The equipment slot to unequip.
       */
      bool unequip(EquipSlot& slot);
};

#endif
