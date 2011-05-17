#ifndef HOME_PANE_H_
#define HOME_PANE_H_

#include "MenuPane.h"

class PlayerData;

class HomePane : public MenuPane
{
   PlayerData& playerData;

   public:
      HomePane(PlayerData& playerData, const gcn::Rectangle& rect);
      ~HomePane();
};

#endif