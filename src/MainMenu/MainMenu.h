#ifndef __MAIN_MENU_H_
#define __MAIN_MENU_H_

#include "SDL_mixer.h"

#include "GameState.h"

namespace gcn
{  class Image;
   class Icon;
};

namespace edwt
{
   class StringListModel;
   class TitleScreenListBox;
   class OpenGLTrueTypeFont;
};

/**
 * Provides the title screen functionality, including the game logo, backdrop, 
 * and options for the user.
 * This state is the first that the user must interact with.
 *
 * @author Noam Chitayat
 */
class MainMenu: public GameState
{  /** Main menu music */
   Mix_Music *music;

   /** Sound for hovering over an option */
   Mix_Chunk *reselectSound;

   /** Sound for picking an option */
   Mix_Chunk *chooseSound;

   /** The list box for all options in the title screen */
   edwt::TitleScreenListBox* listbox;

   /** The background image */
   gcn::Image* bgImg;

   /** The container for the background image */
   gcn::Icon* bg;

   /** The list model holding the options for the title screen */
   edwt::StringListModel* titleOps;

   /** The font for the title screen menu options */
   edwt::OpenGLTrueTypeFont* titleFont;

   /**
    * Populate the title screen list with required options
    */
   void populateOpsList();

    //Actions for the list ops - see documentation in MainMenuActions.cpp
    void NewGameAction();
    void LoadGameAction();
    void OptionsAction();
    void AboutAction();
    void QuitAction();

   public:
      /**
       * Constructor.
       *
       * Initializes the title screen widgets, font, image and sounds
       */
      MainMenu();

      /**
       * Destructor.
       */
      ~MainMenu();

      /**
       * Just draws widgets, flips the buffer,
       * and then waits a millisecond (no rush on a title screen)
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

      /**
       * When the state is activated, set modal focus to the listbox
       */
      void activate();
};

#endif
