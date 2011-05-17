#ifndef EQUIP_PANE_H_
#define EQUIP_PANE_H_

#include "MenuPane.h"

class PlayerData;

class EquipPane : public MenuPane
{
   PlayerData& playerData;

   public:
      EquipPane(PlayerData& playerData, const gcn::Rectangle& rect);
      ~EquipPane();
};

#endif