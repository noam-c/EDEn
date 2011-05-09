#ifndef __MENU_H_
#define __MENU_H_

#include "GameState.h"

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

class PlayerData;
class Sound;

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
   const PlayerData& playerData;

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
      Menu(const PlayerData& playerData);

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
