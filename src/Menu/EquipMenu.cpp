#include "EquipMenu.h"
#include "EquipPane.h"
#include "PlayerData.h"
#include "MenuShell.h"

EquipMenu::EquipMenu(MenuShell& menuShell, PlayerData& playerData, const std::string& characterName) : MenuState(menuShell), playerData(playerData), characterName(characterName)
{
   setMenuPane(new EquipPane(playerData.getPartyCharacter(characterName), menuShell.getDimension()));
}

void EquipMenu::tabChanged(const std::string& tabName)
{
   if(tabName == "Party")
   {
      finished = true;
   }
   else
   {
      setCharacter(tabName);
   }
}

void EquipMenu::setCharacter(const std::string& charName)
{
   characterName = charName;
   ((EquipPane*)menuPane)->setCharacter(playerData.getPartyCharacter(charName));
}

EquipMenu::~EquipMenu()
{
}
