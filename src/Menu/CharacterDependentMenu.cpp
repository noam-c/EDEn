#include "CharacterDependentMenu.h"



CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, PlayerData& playerData, const std::string& stateName) :
   MenuState(gameContext, playerData, stateName)
{
}

CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, const std::string& stateName, MenuShell* menuShell) :
   MenuState(gameContext, stateName, menuShell)
{
}

CharacterDependentMenu::~CharacterDependentMenu()
{
}
