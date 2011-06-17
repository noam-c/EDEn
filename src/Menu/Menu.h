#ifndef MENU_H
#define MENU_H

#include "GameState.h"
#include "CharacterSelectListener.h"
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
   class Container;
   class TabbedArea;
};

enum MenuAction
{
   NONE,
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

class HomePane;

/**
 * \todo Document.
 *
 * @author Noam Chitayat
 */
class Menu: public GameState, public CharacterSelectListener
{
   /** Sound for hovering over an option */
   Sound* reselectSound;

   /** Sound for picking an option */
   Sound* chooseSound;

   /** Background for the menu */
   gcn::Icon* bg;

   /** The tabbed container for the main area of the menu */
   edwt::TabbedArea* menuTabs;

   /** The main area of the menu */
   edwt::Container* menuArea;

   /** The list box for all options in the menu */
   edwt::ListBox* actionsListBox;

   /** The list model holding the options for the menu */
   edwt::StringListModel* listOps;

   /** The player data */
   PlayerData& playerData;

   /** The home panel to display */
   HomePane* homePanel;

   /** The character-dependent action that is in queue after a character is selected */
   MenuAction characterAction;

   /**
    * Populate the menu action list with required options
    */
   void populateOpsList();

   /**
    * Poll for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void pollInputEvent(bool& finishState);

   /**
    * @return true iff the action requires a character to be selected
    */
   bool isCharacterDependent(MenuAction action);

   /**
      * Show the specified panel, and hide the currently active panel.
      */
   void showPanel(MenuAction panelToShow);

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
       * Called when a character is selected from the home pane.
       *
       * @param characterName the name of the character that was selected
       */
      void characterSelected(const std::string& characterName);

      /**
       * Destructor.
       */
      ~Menu();
};

#endif
