#include "StatusMenu.h"
#include "StatusPane.h"
#include "PlayerData.h"
#include "MenuShell.h"

StatusMenu::StatusMenu(MenuShell& menuShell, PlayerData& playerData, const std::string& characterName) : MenuState(menuShell), playerData(playerData), characterName(characterName)
{
   setMenuPane(new StatusPane(playerData.getPartyCharacter(characterName), menuShell.getDimension()));
}

void StatusMenu::tabChanged(const std::string& tabName)
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

void StatusMenu::setCharacter(const std::string& charName)
{
   characterName = charName;
   ((StatusPane*)menuPane)->setCharacter(playerData.getPartyCharacter(charName));
}

StatusMenu::~StatusMenu()
{
}