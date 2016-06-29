/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "CharacterDependentMenu.h"

CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, PlayerData& playerData, const std::string& stateName) :
   MenuState(gameContext, playerData, stateName)
{
}

CharacterDependentMenu::CharacterDependentMenu(GameContext& gameContext, PlayerData& playerData, const std::string& stateName, std::shared_ptr<MenuShell> menuShell) :
   MenuState(gameContext, playerData, stateName, std::move(menuShell))
{
}
