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
   void serializePortraitData(Json::Value& characterNode);
   
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
      void serialize(Json::Value& characterSet);
   
      /**
       * @return The name of the character.
       */
      std::string getName();
   
      /**
       * @return The relative file path containing the picture of this character.
       * \todo The character portrait data should just contain an ID for the picture instead of a filepath.
       */
      std::string getPortraitPath();

      // Getters for the health and stamina.
      int getMaxHP();
      int getMaxSP();
      int getHP();
      int getSP();

      // Getters for the character attributes.
      int getStrength();
      int getIntelligence();
      int getVitality();
      int getReflex();
      int getFocus();
      int getEndurance();
      int getAgility();
      
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
