#ifndef EQUIP_MENU_H
#define EQUIP_MENU_H

#include "MenuState.h"
#include <string>

class EquipPane;
class PlayerData;
class Character;

class EquipMenu : public MenuState
{
   std::string characterName;
   PlayerData& playerData;

   public:
      EquipMenu(MenuShell& menuShell, PlayerData& playerData, const std::string& characterName);
      void tabChanged(const std::string& tabName);
      void setCharacter(const std::string& charName);
      ~EquipMenu();
};

#endif
