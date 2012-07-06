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
#include "Tab.h"

StatusMenu::StatusMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData, Character* character) : MenuState(executionStack, menuShell), playerData(playerData), character(character)
{
   setMenuPane(new StatusPane(character, menuShell.getDimension()));
}

void StatusMenu::tabChanged(int index)
{
   if(index <= 0)
   {
      finished = true;
   }
   else
   {
      setCharacter(playerData.getRoster()->getParty()[index - 1]);
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
