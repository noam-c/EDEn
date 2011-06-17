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
class CharacterSelectListener;

class CharacterModule : public gcn::contrib::AdjustingContainer, public gcn::MouseListener
{
   gcn::contrib::AdjustingContainer* characterStats;

   CharacterSelectListener* charSelectListener;
   std::string characterName;
   edwt::Label* characterNameLabel;
   edwt::Label* characterHPLabel;
   edwt::Label* characterSPLabel;
   edwt::Icon* characterPortrait;

   public:
      CharacterModule();
      void mouseClicked(gcn::MouseEvent& mouseEvent);
      void setCharacterSelectListener(CharacterSelectListener* listener);
      void setCharacter(Character* character);
      ~CharacterModule();
};

#endif