/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef TITLE_SCREEN_SETTINGS_MENU_H
#define TITLE_SCREEN_SETTINGS_MENU_H

#include "GameState.h"
#include "EdenRocketBindings.h"
#include "Scheduler.h"

#include <memory>

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
class Settings;

/**
 * Provides the title screen settings menu, allowing the user to
 * modify game settings.
 *
 * @author Noam Chitayat
 */
class TitleScreenSettingsMenu final : public GameState
{
   /** The event binding collection for this GUI */
   EdenRocketBindings m_bindings;

   /** The coroutine scheduler for the main menu's GUI scripts */
   Scheduler m_scheduler;

   /** The title screen RML document */
   Rocket::Core::ElementDocument* m_titleSettingsDocument;

   /** The music checkbox */
   Rocket::Core::Element* m_musicEnabledCheckbox;

   /** The sound checkbox */
   Rocket::Core::Element* m_soundEnabledCheckbox;

   /** The fullscreen checkbox */
   Rocket::Core::Element* m_fullScreenEnabledCheckbox;

   /**
    * Poll and handle the next input event.
    *
    * @param finishState Returned as true if the input event quit out of the main menu.
    */
   void waitForInputEvent(bool& finishState);

   static bool getCheckboxValue(Rocket::Core::Event& event);
   void loadSettings();
   void saveSettings();
   void revertSettings();

   void onMusicEnabledChange(Rocket::Core::Event& event);
   void onSoundEnabledChange(Rocket::Core::Event& event);
   void onFullScreenEnabledChange(Rocket::Core::Event& event);
   void onSubmit(Rocket::Core::Event& event);

   protected:
      /**
       * Waits a millisecond between draws (no rush on a title screen)
       */
      void draw() override;

      /**
       * Perform logic for the title screen.
       * NOTE: this method will spin infinitely until user input is received.
       * Since the screen is static, there is no need to refresh without input.
       *
       * @return true iff the title screen is not finished running (no quit event)
       */
      bool step(long timePassed) override;

      /**
       * @return the title screen settings menu's script scheduler
       */
      Scheduler* getScheduler() override;

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
      ~TitleScreenSettingsMenu() override;
};

#endif
