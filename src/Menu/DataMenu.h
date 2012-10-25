/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef DATA_MENU_H
#define DATA_MENU_H

#include "MenuState.h"
#include "EdenRocketBindings.h"
#include "DataViewModel.h"

namespace Rocket
{
   namespace Core
   {
      class Context;
      class ElementDocument;
      class Element;
      class Event;
   };
};

class PlayerData;
class Sound;

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
   EdenRocketBindings<DataMenu> bindings;

   /** The player data */
   PlayerData& playerData;

   /** The view model */
   DataViewModel dataViewModel;

   /** The data menu RML document */
   Rocket::Core::ElementDocument* paneDocument;

   /** The RML document for the Confirm Save window in the data menu */
   Rocket::Core::ElementDocument* confirmSaveDocument;

   int slotToSave;

   void showConfirmDialog(int index);
   void hideConfirmDialog();
   void confirmClicked(Rocket::Core::Event* event);
   void cancelClicked(Rocket::Core::Event* event);
   void saveGameClicked(Rocket::Core::Event* event);

   public:
      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param playerData The player data that the menu will display.
       */
      DataMenu(ExecutionStack& executionStack, PlayerData& playerData);

      /**
       * Constructor. Initializes the menu GUI.
       *
       * @param executionStack The execution stack that the state belongs to.
       * @param playerData The player data that the menu will display.
       * @param menuShell The shell for the menu
       */
      DataMenu(ExecutionStack& executionStack, PlayerData& playerData, MenuShell* menuShell);

      void initialize();
      void activate();
      void deactivate();

      std::vector<MenuShellOption> getSidebarOptions();

      void sidebarClicked(int optionIndex);

      /**
       * Destructor.
       */
      ~DataMenu();
};

#endif
