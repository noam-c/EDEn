/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef TITLE_SCREEN_SETTINGS_MENU_H
#define TITLE_SCREEN_SETTINGS_MENU_H

#include "GameState.h"
#include "EdenRocketBindings.h"

namespace Rocket
{
   namespace Core
   {
      class ElementDocument;
      class Event;
   };
};

class Music;
class Sound;

/**
 * Provides the title screen settings menu, allowing the user to
 * modify game settings.
 *
 * @author Noam Chitayat
 */
class TitleScreenSettingsMenu: public GameState
{
   /** The event binding collection for this GUI */
   EdenRocketBindings<TitleScreenSettingsMenu> bindings;

   /** The coroutine scheduler for the main menu's GUI scripts */
   Scheduler* scheduler;

   /** The title screen RML document */
   Rocket::Core::ElementDocument* titleSettingsDocument;

   /**
    * Poll and handle the next input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

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
       * @return the title screen settings menu's script scheduler
       */
      Scheduler* getScheduler() const;

   public:
      /**
       * Constructor.
       * Initializes the title screen settings menu widgets, font, image and sounds.
       *
       * @param gameContext The context containing the current player data and execution stack.
       */
      TitleScreenSettingsMenu(GameContext& gameContext);
   
      /**
       * Destructor.
       */
      ~TitleScreenSettingsMenu();
};

#endif
