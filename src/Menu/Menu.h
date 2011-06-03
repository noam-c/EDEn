#ifndef MENU_H
#define MENU_H

#include "GameState.h"
#include <stack>
#include <map>

namespace gcn
{
   class Image;
   class Icon;
};

namespace edwt
{
   class StringListModel;
   class ListBox;
   class Label;
   class OpenGLTrueTypeFont;
};

enum MenuPanelType
{
	HOME_PANEL,
   ITEM_PANEL,
	EQUIP_PANEL,
	STATUS_PANEL,
	SKILLS_PANEL,
	FORMATION_PANEL,
	PARTY_CHANGE_PANEL,
	OPTIONS_PANEL,
	DATA_PANEL,
};

class PlayerData;
class Sound;

class MenuPane;
class ItemsPane;
class HomePane;
class EquipPane;
class CharacterPane;

/**
 * \todo Document.
 *
 * @author Noam Chitayat
 */
class Menu: public GameState
{
   /** Sound for hovering over an option */
   Sound* reselectSound;

   /** Sound for picking an option */
   Sound* chooseSound;

   /** The list box for all options in the menu */
   edwt::ListBox* actionsListBox;

   /** The list model holding the options for the menu */
   edwt::StringListModel* listOps;

   /** The player data */
   PlayerData& playerData;

   /** Stack of active panels. Top panel is the one currently being shown. */
   std::stack<MenuPanelType> activePaneStack;

   /** Map of panes */
   std::map<MenuPanelType, MenuPane*> menuPanes;

   /**
    * Populate the menu action list with required options
    */
   void populateOpsList();

   /**
    * Wait for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   /**
      * Show the specified panel, and hide the currently active panel.
      */
   void showPanel(MenuPanelType panelToShow);

   /**
      * Show the specified panel, and hide the currently active panel.
      */
   void popPanel();

   protected:
      /**
       * Waits a millisecond between draws (no rush on a menu screen)
       */
      void draw();

      /**
       * Perform logic for the menu screen.
       * NOTE: this method will spin wait until user input is received.
       * Since the screen is static, there is no need to refresh without input.
       *
       * @return true iff the title screen is not finished running (no quit event)
       */
      bool step();

   public:
      /**
       * Constructor.
       *
       * Initializes the menu widgets, font, image and sounds
       */
      Menu(PlayerData& playerData);

      /**
       * When the state is activated, set modal focus to the listbox
       */
      void activate();

      /**
       * Destructor.
       */
      ~Menu();
};

#endif
