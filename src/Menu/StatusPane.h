#ifndef STATIS_PANE_H_
#define STATIS_PANE_H_

#include "MenuPane.h"

class CharacterModule;
class Character;

class StatusPane : public MenuPane
{
   Character& character;
   CharacterModule* module;

   public:
      StatusPane(Character& character, const gcn::Rectangle& rect);
      ~StatusPane();
};

#endif