#ifndef DATA_MENU_H
#define DATA_MENU_H

#include "MenuState.h"
#include "guichan.hpp"
#include "ModuleSelectListener.h"
#include "ConfirmStateListener.h"

class PlayerData;

/**
 * The data pane displays a list of savegames in the player's savegames folder, and allows the player to save to
 * these files.
 *
 * @author Noam Chitayat
 */
class DataMenu : public MenuState, public ModuleSelectListener, public ConfirmStateListener
{
   /** The player data that the menu interacts with. */
   PlayerData& playerData;
   
   std::vector<PlayerData*> saveGames;
   
   /** Selected save game when the confirmation dialog is raised */
   std::string selectedSavePath;
   
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
       * Refreshes the savegame directory and sends the savegame data to the display.
       */
      void refresh();
   
      /**
       * Signals that a save game was selected in the data menu.
       *
       * @param index The index of the save game to save over.
       */
      void moduleSelected(int index);
      
      /**
       * 'Yes' was clicked in the confirmation dialog.
       * Save the selected game.
       */
      void yesClicked();
      
      /**
       * 'No' was clicked in the confirmation dialog.
       * Do not save the selected game.
       */
      void noClicked();
   
      /**
       * Destructor.
       */
      ~DataMenu();
};

#endif
