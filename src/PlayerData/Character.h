#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>

namespace Json
{
   class Value;
};

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
      Character(Json::Value& charToLoad);
      void parsePortraitData(Json::Value& charToLoad);
      void serialize(Json::Value& characterSet);
      void serializePortraitData(Json::Value& characterSet);

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