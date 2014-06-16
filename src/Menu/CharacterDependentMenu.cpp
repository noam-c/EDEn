#include "CharacterDependentMenu.h"



CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, PlayerData& playerData, const std::string& stateName) :
   MenuState(gameContext, playerData, stateName)
{
}

CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, PlayerData& playerData, const std::string& stateName, std::shared_ptr<MenuShell> menuShell) :
   MenuState(gameContext, playerData, stateName, menuShell)
{
}
