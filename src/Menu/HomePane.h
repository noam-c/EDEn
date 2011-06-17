#ifndef HOME_PANE_H_
#define HOME_PANE_H_

#include "MenuPane.h"

class PlayerData;
class CharacterModule;
class CharacterSelectListener;

class HomePane : public MenuPane
{
   PlayerData& playerData;
   CharacterModule* characterModules;

   public:
      HomePane(PlayerData& playerData, const gcn::Rectangle& rect);
      void setCharacterSelectListener(CharacterSelectListener* listener);
      void refresh();
      ~HomePane();
};

#endif