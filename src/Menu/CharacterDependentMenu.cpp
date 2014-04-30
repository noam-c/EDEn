#include "CharacterDependentMenu.h"



CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, const std::string& stateName) :
   MenuState(gameContext, stateName)
{
}

CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, const std::string& stateName, std::shared_ptr<MenuShell> menuShell) :
   MenuState(gameContext, stateName, menuShell)
{
}
