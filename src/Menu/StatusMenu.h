#ifndef STATUS_MENU_H
#define STATUS_MENU_H

#include "MenuState.h"
#include <string>

class StatusPane;
class PlayerData;
class Character;

class StatusMenu : public MenuState
{
   std::string characterName;
   PlayerData& playerData;

   public:
      StatusMenu(MenuShell& menuShell, PlayerData& playerData, const std::string& characterName);
      void tabChanged(const std::string& tabName);
      void setCharacter(const std::string& charName);
      ~StatusMenu();
};

#endif