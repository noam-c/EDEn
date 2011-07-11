#ifndef MENU_H
#define MENU_H

#include "MenuState.h"
#include "CharacterSelectListener.h"
#include "MenuAction.h"
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

class PlayerData;
class Sound;
class MenuShell;

/**
 * The home menu is the first menu state of the menu.
 * It serves to summarize important information for the player and allows the player to select
 * a character to perform further actions on.
 *
 * @author Noam Chitayat
 */
class HomeMenu: public MenuState, public CharacterSelectListener
{
   /** The player data */
   PlayerData& playerData;

   /** The character-dependent action that is in queue after a character is selected */
   MenuAction characterAction;

   /**
    * Poll for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main HomeMenu.
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
       * Perform logic for the HomeMenu screen.
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
       * Initializes the HomeMenu widgets, font, image and sounds
       */
      HomeMenu(MenuShell& top, PlayerData& playerData);

      /**
       * Called when a character is selected from the home pane.
       *
       * @param characterName the name of the character that was selected
       */
      void characterSelected(const std::string& characterName);

      /**
       * Called when a new tab in a tabbed area is selected.
       *
       * @param tabName the name of the tab that was selected.
       */
      void tabChanged(const std::string& tabName);

      /**
       * Destructor.
       */
      ~HomeMenu();
};

#endif
