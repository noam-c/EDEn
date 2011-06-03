#ifndef __MAIN_MENU_H_
#define __MAIN_MENU_H_

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

class Music;
class Sound;

/**
 * Provides the title screen functionality, including the game logo, backdrop, 
 * and options for the user.
 * This state is the first that the user must interact with.
 *
 * @author Noam Chitayat
 */
class MainMenu: public GameState
{
   /** Main menu music */
   Music* music;

   /** Sound for hovering over an option */
   Sound* reselectSound;

   /** Sound for picking an option */
   Sound* chooseSound;

   /** The main title on top of the title screen */
   edwt::Label* titleLabel;

   /** The list box for all options in the title screen */
   edwt::ListBox* actionsListBox;

   /** The container for the background image */
   gcn::Icon* bg;

   /** The list model holding the options for the title screen */
   edwt::StringListModel* titleOps;

   /** The font for the title screen heading */
   edwt::OpenGLTrueTypeFont* titleFont;

   /** The font for the title screen menu options */
   edwt::OpenGLTrueTypeFont* actionsFont;

   /**
    * Populate the title screen list with required options
    */
   void populateOpsList();

   /**
    * Wait for and handle the input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   //Actions for the list ops - see documentation in MainMenuActions.cpp
   void NewGameAction();
   void MenuPrototypeAction();
   void LoadGameAction();
   void OptionsAction();
   void AboutAction();
   void QuitAction();

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
      bool step();

   public:
      /**
       * Constructor.
       *
       * Initializes the title screen widgets, font, image and sounds
       */
      MainMenu();

      /**
       * When the state is activated, set modal focus to the listbox
       */
      void activate();

      /**
       * Destructor.
       */
      ~MainMenu();
};

#endif
