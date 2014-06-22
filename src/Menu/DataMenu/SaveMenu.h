/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SAVE_LOAD_MENU_H
#define SAVE_LOAD_MENU_H

#include "GameState.h"
#include "EdenRocketBindings.h"
#include "SaveGameModel.h"
#include "SaveGameViewModel.h"
#include "PlayerData.h"
#include "Scheduler.h"

#include <memory>

namespace Rocket
{
   namespace Core
   {
      class ElementDocument;
      class Event;
   };
};

typedef std::vector<std::pair<std::string, std::unique_ptr<PlayerDataSummary>> > SaveGameList;

/**
 * Provides the title screen functionality, including the game logo, backdrop,
 * and options for the user.
 * This state is the first that the user must interact with.
 *
 * @author Noam Chitayat
 */
class SaveMenu: public GameState, public std::enable_shared_from_this<SaveMenu>
{
   /** The event binding collection for this GUI */
   EdenRocketBindings m_bindings;
   
   /** The coroutine scheduler for the main menu's GUI scripts */
   Scheduler m_scheduler;
   
   /** The title screen RML document */
   Rocket::Core::ElementDocument* m_titleDocument;
   
   /** The location of the player in the game world. */
   const SaveLocation m_saveLocation;
   
   SaveGameModel m_model;
   
   /** The current player data. */
   PlayerData& m_playerData;
   
   /** The list of savegame files and their respective data. */
   SaveGameList m_saveGames;
   
   /** The view model that exposes the savegames to the GUI */
   SaveGameViewModel m_saveGameViewModel;
   
   /** The RML document for the Confirm Save window in the data menu */
   Rocket::Core::ElementDocument* m_confirmSaveDocument;
   
   /** The slot to save to after the user confirms the save. */
   int m_slotToSave;
   
   /**
    * Poll and handle the next input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);
   
   /**
    * Callback for Rocket event when a key is pressed
    * while the list of menu options has focus.
    *
    * @param event The user input event.
    */
   void listKeyDown(Rocket::Core::Event& event);
   
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
   
protected:
   /**
    * Waits a millisecond between draws (no rush on a title screen)
    */
   void draw();
   
   /**
    * Perform logic for the title screen.
    * NOTE: this method will spin infinitely until user input is received.
    * Since the screen is static, there is no need to refresh without input.
    *
    * @return true iff the title screen is not finished running (no quit event)
    */
   bool step(long timePassed);
   
   /**
    * @return the main menu's script scheduler
    */
   Scheduler* getScheduler();
   
   public:
      /**
       * Constructor.
       * Initializes the title screen widgets, font, image and sounds.
       *
       * @param gameContext The context containing the current player data and execution stack.
       */
      SaveMenu(GameContext& gameContext, PlayerData& playerData, const SaveLocation saveLocation);
      
      /**
       * Destructor.
       */
      ~SaveMenu();
      
      void refresh(int slotIndex);
};

#endif
