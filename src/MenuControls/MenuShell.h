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
class MenuShell : public edwt::Container
{
   /** Sound for hovering over an option */
   Sound* reselectSound;

   /** Sound for picking an option */
   Sound* chooseSound;

   /** Background for the menu */
   gcn::Icon* bg;

   /** The list model holding the options for the menu */
   edwt::StringListModel* listOps;

   /** The tabbed container for the main area of the menu */
   edwt::TabbedArea* menuTabs;
      
   /** The main area of the menu */
   edwt::Container* menuArea;

   /**
    * Populate the action list with required options
    */
   void populateOpsList();

   public:
      /** \todo THIS PUBLIC FIELD IS LIKE OMG TEMPORARY. REMOVE IMMEDIATELY! */
      /** The list box for all options in the menu */
      edwt::ListBox* actionsListBox;

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
       * Set the state or GUI element that will capture character selections using the menu tabs.
       */
      void setTabChangeListener(edwt::TabChangeListener* listener);

      /**
       * Destructor.
       */
      ~MenuShell();

};

#endif
