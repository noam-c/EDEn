/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef MENU_SHELL_H
#define MENU_SHELL_H

#include "Container.h"
#include "MenuAction.h"

namespace gcn
{
   class Icon;
};

namespace edwt
{
   class TabbedArea;
   class ListBox;
   class StringListModel;
   class TabChangeListener;
};

class MenuState;
class MenuPane;
class PlayerData;
class Sound;

/**
 * The menu shell contains the foundations of the in-game menu.
 * All the standard GUI elements, such as the action list and the tabbed area containing the various menu panes
 * are created in the shell, which is passed up through any menu-based GameState (MenuState types) to allow for
 * sharing of a single shell among states.
 *
 * @author Noam Chitayat
 */
class MenuShell : public edwt::Container, public gcn::ActionListener, public gcn::SelectionListener
{
   /** Sound for hovering over an option */
   Sound* selectSound;

   /** Background for the menu */
   gcn::Icon* bg;

   /** The list box for all options in the menu */
   edwt::ListBox* actionsListBox;
   
   /** The list model holding the options for the menu */
   edwt::StringListModel* listOps;

   /** The tabbed container for the main area of the menu */
   edwt::TabbedArea* menuTabs;
      
   /** The main area of the menu */
   edwt::Container* menuArea;

   /** The active menu state */
   MenuState* activeState;
   
   /**
    * Populate the action list with required options
    */
   void populateOpsList();

   public:
      /**
       * Constructor.
       *
       * @param playerData The player data model accessed by this menu.
       */
      MenuShell(PlayerData& playerData);

      /**
       * Push a menu pane to the central part of the menu shell.
       *
       * @param menuPane The menu pane to display inside the menu shell.
       */
      void addPane(MenuPane* menuPane);

      /**
       * Remove a menu pane from the menu shell.
       *
       * @param menuPane The menu pane to remove from the menu shell display.
       */
      void removePane(MenuPane* menuPane);

      /**
       * Sets the menu state that will listen for events from
       * the action list.
       *
       * @param menuState The menu state that will respond to events.
       */
      void setActiveState(MenuState* menuState);
   
      /**
       * Picks up and sends action events to the current menu state.
       *
       * @param event The action event to send to the menu state.
       */
      void action(const gcn::ActionEvent& event);
      
      /**
       * Responds to selection events from the listbox.
       *
       * @param event The selection event to respond to.
       */
      void valueChanged(const gcn::SelectionEvent& event);
   
      /**
       * Destructor.
       */
      ~MenuShell();

};

#endif
