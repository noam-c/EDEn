/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_SDL_INPUT_MAPPING_H
#define ROCKET_SDL_INPUT_MAPPING_H

#include <SDL.h>
#include <Rocket/Core.h>
#include <map>

/**
 * Adaptor layer that serves to map SDL input events to
 * Rocket events that can be sent to a Rocket context for
 * proper GUI processing.
 *
 * @author Noam Chitayat
 */
class RocketSDLInputMapping
{
   /**
    * The mapping between the SDLKey keyboard input enum to the
    * Rocket KeyIdentifier enum.
    */
   static std::map<SDL_Keycode, Rocket::Core::Input::KeyIdentifier> keyMap;

   /**
    * @param modifiers The SDL keyboard event's keyboard modifiers.
    *
    * @return The Rocket representation of the keyboard modifiers currently pressed by the user.
    */
   static int getKeyModifierState(const SDL_Keymod& modifiers);

   /**
    * @param mouseButtonEvent The SDL event structure for the user's mouse click.
    *
    * @return The Rocket mouse button index for the clicked mouse button.
    */
   static int mapMouseButton(const SDL_MouseButtonEvent& mouseButtonEvent);

   public:
      /**
       * Initializes the SDL-to-Rocket mappings.
       */
      static void initialize();

      /**
       * Pipes an SDL event into the given Rocket context.
       *
       * @param context The context to send the SDL input event to.
       * @param event The input event to send to the Rocket context (after proper mapping).
       */
      static void handleSDLEvent(Rocket::Core::Context* context, const SDL_Event& event);
};

#endif
