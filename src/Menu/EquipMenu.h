#ifndef EQUIP_MENU_H
#define EQUIP_MENU_H

#include "MenuState.h"
#include <string>

class EquipPane;
class PlayerData;
class Character;

/**
 * The menu state that allows the player to view and change the equipment of characters in the party.
 *
 * @author Noam Chitayat
 */
class EquipMenu : public MenuState
{
   /** The player data model to operate on. */
   PlayerData& playerData;
   
   /** The name of the character to display equipment info for. */
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
      EquipMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData, const std::string& characterName);
  
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
      ~EquipMenu();
};

#endif
