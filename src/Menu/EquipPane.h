#ifndef EQUIP_PANE_H_
#define EQUIP_PANE_H_

#include "MenuPane.h"

namespace edwt
{
   class Container;
   class Label;
};

class Character;
class PlayerData;

class EquipPane : public MenuPane
{
   Character* character;
   edwt::Label* headLabel;
   edwt::Label* headEquipLabel;
 
   public:
      EquipPane(Character* character, const gcn::Rectangle& rect);
      void setCharacter(Character* character);
      ~EquipPane();
};

#endif
