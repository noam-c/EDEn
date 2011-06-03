#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include <string>

class TiXmlElement;

class Character
{
   std::string name;

   std::string portraitPath;

   int strength;
   int intelligence;
   int vitality;
   int reflex;
   int focus;
   int endurance;
   int agility;

   int maxHP;
   int maxSP;
   int hp;
   int sp;
   
   public:
      Character(const std::string& name);
      Character(TiXmlElement* charToLoad);
      void parsePortraitData(TiXmlElement* charToLoad);

      void serialize(TiXmlElement& characterSet);
      void serializePortraitData(TiXmlElement& characterSet);

      std::string getName();
      std::string getPortraitPath();

      int getMaxHP();
      int getMaxSP();
      int getHP();
      int getSP();

      int getStrength();
      int getIntelligence();
      int getVitality();
      int getReflex();
      int getFocus();
      int getEndurance();
      int getAgility();
};

#endif