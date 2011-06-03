#ifndef CHARACTER_MODULE_H_
#define CHARACTER_MODULE_H_

#include "guichan.hpp"

namespace edwt
{
   class Container;
   class Icon;
   class Label;
};

class Character;

class CharacterModule : public gcn::contrib::AdjustingContainer
{
   gcn::contrib::AdjustingContainer* characterStats;

   edwt::Label* characterName;
   edwt::Label* characterHPLabel;
   edwt::Label* characterSPLabel;
   edwt::Icon* characterPortrait;

   public:
      CharacterModule();
      void setCharacter(Character* character);
      ~CharacterModule();
};

#endif