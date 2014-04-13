/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef DATA_MENU_H
#define DATA_MENU_H

#include "MenuState.h"
#include "EdenRocketBindings.h"
#include "DataViewModel.h"

#include <memory>

namespace Rocket
{
   namespace Core
   {
      class Context;
      class Element;
      class Event;
   };
};

class Sound;
typedef std::vector<std::pair<std::string, std::unique_ptr<PlayerDataSummary>> > SaveGameList;

/**
 * The home menu is the first menu state of the menu.
 * It serves to summarize important information for the player and allows the player to select
 * a character to perform further actions on.
 *
 * @author Noam Chitayat
 */
class DataMenu: public MenuState
{
   /** The event binding collection for this GUI */
   EdenRocketBindings m_bindings;

   /** The list of savegame files and their respective data. */
   SaveGameList m_saveGames;
   
   /** The view model that exposes the savegames to the GUI */
   DataViewModel m_dataViewModel;

   /** The RML document for the Confirm Save window in the data menu */
   Rocket::Core::ElementDocument* m_confirmSaveDocument;

   /** The slot to save to after the user confirms the save. */
   int m_slotToSave;

   /**
    * Displays the confirmation dialog to ensure that the user wants
    * to save at this location.
    *
    * @param index The index of the data to save.
    */
   void showConfirmDialog(int index);

   /**
    * Hides the confirmation dialog if it is visible, and
    * resets the save slot.
    */
   void hideConfirmDialog();

   /**
    * Event handler for the "Yes"/"Confirm" button being clicked.
    *
    * @param event The Rocket GUI event being sent by the user input.
    */
   void confirmClicked(Rocket::Core::Event& event);

   /**
    * Event handler for the "No"/"Cancel" button being clicked.
    *
    * @param event The Rocket GUI event being sent by the user input.
    */
   void cancelClicked(Rocket::Core::Event& event);

   /**
    * Event handler for a savegame being clicked.
    * Begins the saving process using the clicked slot.
    *
    * @param event The Rocket GUI event that indicates which slot was clicked.
    */
   void saveGameClicked(Rocket::Core::Event& event);
   
   /**
    * Refreshes the list of save games.
    */
   void refreshSaveGames();
   
   /**
    * Saves the current player data to the given save slot.
    *
    * @param slotIndex The slot at which the game will be saved.
    */
   void saveToSlot(int slotIndex);

   /**
    * Initializes the data menu pane and populates the sidebar.
    */
   void initialize();

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       */
      DataMenu(GameContext& gameContext);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param gameContext The context containing the current player data and execution stack.
       * @param menuShell The shell for the menu
       */
      DataMenu(GameContext& gameContext, std::shared_ptr<MenuShell> menuShell);

      /**
       * Destructor.
       */
      ~DataMenu();

      /**
       * Handles a sidebar click for the data menu.
       * \todo Finish implementation of this method.
       *
       * @param optionIndex The index of the sidebar option that was clicked.
       */
      void sidebarClicked(int optionIndex);
   
      const SaveGameList& getSaveGames() const;
};

#endif
