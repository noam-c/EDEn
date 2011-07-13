#ifndef DATA_MENU_H
#define DATA_MENU_H

#include "MenuState.h"

class PlayerData;

/**
 * The data pane displays a list of savegames in the player's savegames folder, and allows the player to save to
 * these files.
 *
 * @author Noam Chitayat
 */
class DataMenu : public MenuState
{
   /** The player data that the menu interacts with. */
   PlayerData& playerData;
   
   public:
      /**
       * Constructor.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param menuShell The shell to display the data menu GUI in.
       * @param playerData The player data to operate on.
       */
      DataMenu(ExecutionStack& executionStack, MenuShell& menuShell, PlayerData& playerData);

      /**
       * Destructor.
       */
      ~DataMenu();
};

#endif
