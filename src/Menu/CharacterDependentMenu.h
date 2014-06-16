#ifndef CHARACTER_DEPENDENT_MENU_H
#define CHARACTER_DEPENDENT_MENU_H

#include "MenuState.h"

class CharacterDependentMenu : public MenuState
{
   public:

      CharacterDependentMenu(GameContext& gameContext, PlayerData& playerData, const std::string& stateName);
      CharacterDependentMenu(GameContext& gameContext, PlayerData& playerData, const std::string& stateName, std::shared_ptr<MenuShell> menuShell);
      virtual void setCharacter(int characterIndex) = 0;
      virtual ~CharacterDependentMenu() = default;
};

#endif
