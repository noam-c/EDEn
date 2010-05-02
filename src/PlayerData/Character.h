#ifndef __CHARACTER_H_
#define __CHARACTER_H_

#include <string>

class TiXmlElement;

class Character
{
   std::string name;
   int characterStrength;
   int characterIntelligence;
   int characterHP;
   int characterSP;
   
   public:
      Character(std::string name);
      Character(TiXmlElement* charToLoad);
      void Character::serialize(TiXmlElement& characterSet);

      std::string getName();
};

#endif