/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef STATUS_MENU_H
#define STATUS_MENU_H

#include "MenuState.h"
#include <string>

class StatusPane;
class PlayerData;
class Character;

/**
 * The StatusMenu represents the game state of displaying a character's attributes and status in the in-game menu.
 * The game enters in this state when the player checks a Character's Status in the menu.
 *
 * @author Noam Chitayat
 */
class StatusMenu : public MenuState
{
   /** The player data operated on by this menu state. */
   PlayerData& playerData;

   /** The name of the character displayed. */
   std::string characterName;

   public:
      /**
       * Constructor.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param menuShell The shell to display the status GUI in.
       * @param playerData The player data to operate on.
       * @param characterName The name of the initial character to display in the status menu.
       */
      StatusMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData, const std::string& characterName);

      /**
       * Fired when the player picks a new character tab.
       * Displays the character in the tab or cancels the status menu if the "Party" tab is selected.
       *
       * @param tabName The name of the newly-selected tab.
       */
      void tabChanged(const std::string& tabName);

      /**
       * Sets the character to be displayed in the menu.
       *
       * @param charName The name of the character to display.
       */
      void setCharacter(const std::string& charName);

      /**
       * Destructor.
       */
      ~StatusMenu();
};

#endif