#ifndef STATUS_MENU_H
#define STATUS_MENU_H

#include "MenuState.h"

class StatusPane;
class PlayerData;
class Character;

class StatusMenu : public MenuState
{
   StatusPane* statusPane;
   Character& character;

   public:
      StatusMenu(edwt::Container* top, edwt::Container& menuPane, Character& character);
      ~StatusMenu();
};

#endif