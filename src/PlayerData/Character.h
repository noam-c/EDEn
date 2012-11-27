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
   /** The character's ID for data lookups. */
   std::string id;

   /** The name of the character. */
   std::string name;

   /** The archetype that was loaded for this character (empty if the default archetype was used). */
   std::string archetype;

   /** The path to the character portrait (used in dialogue and menus). */
   std::string portraitPath;

   /** The ID of the spritesheet resource used for this character. */
   std::string spritesheetId;

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
    * Loads the archetype for the character, which provides character
    * information and base stats/equipment.
    *
    * @param archetypeId The ID of the archetype to load.
    *
    * @return A JSON node containing the loaded character archetype.
    */
   static Json::Value loadArchetype(const std::string& archetypeId);

   /**
    * Parse the portrait path from the character node.
    *
    * @param portraitDataContainer The JSON node containing the portrait data to load.
    */
   void parsePortraitData(Json::Value& portraitDataContainer);
   
   /**
    * Parse the stats from the given node.
    *
    * @param statsDataContainer The JSON node containing the stats to load.
    */
   void parseStats(Json::Value& statsDataContainer);

   public:
      /**
       * Constructor used to create a new character.
       * Initializes the entire character using the base archetype data.
       *
       * @param id The id of the new character.
       */
      Character(const std::string& id);

      /**
       * Constructor used to load an existing character.
       *
       * @param charToLoad The JSON node containing the character's data.
       */
      Character(Json::Value& charToLoad);
   
      /**
       * Destructor.
       */
      ~Character();

      /**
       * Serialize the character data into JSON output.
       *
       * @return A JSON object containing this character's serialized data.
       */
      Json::Value serialize() const;
   
      /**
       * @return The ID of the character.
       */
      std::string getId() const;


      /**
       * @return The name of the character.
       */
      std::string getName() const;

      /**
       * @return The ID of the spritesheet used to represent the character.
       */
      std::string getSpritesheetId() const;

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
