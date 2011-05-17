#ifndef CHARACTER_PANE_H_
#define CHARACTER_PANE_H_

#include "MenuPane.h"

class PlayerData;

class CharacterPane : public MenuPane
{
   PlayerData& playerData;

   public:
      CharacterPane(const gcn::Rectangle& rect);
      ~CharacterPane();
};

#endif