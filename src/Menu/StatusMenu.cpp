/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "StatusMenu.h"
#include "StatusPane.h"
#include "PlayerData.h"
#include "CharacterRoster.h"
#include "MenuShell.h"

StatusMenu::StatusMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData, Character* character) : MenuState(executionStack, menuShell), playerData(playerData), character(character)
{
   setMenuPane(new StatusPane(character, menuShell.getDimension()));
}

void StatusMenu::tabChanged(const std::string& tabName)
{
   if(tabName == "Party")
   {
      finished = true;
   }
   else
   {
      setCharacter(playerData.getRoster()->getCharacter(tabName));
   }
}

void StatusMenu::setCharacter(Character* newCharacter)
{
   character = newCharacter;
   ((StatusPane*)menuPane)->setCharacter(character);
}

StatusMenu::~StatusMenu()
{
}
