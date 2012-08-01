/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_SDL_INPUT_MAPPING_H
#define ROCKET_SDL_INPUT_MAPPING_H

#include "SDL.h"
#include <Rocket/Core.h>
#include <map>

class RocketSDLInputMapping
{
   static std::map<SDLKey, Rocket::Core::Input::KeyIdentifier> keyMap;

   static int getKeyModifierState(const SDLMod& modifiers);
   static int mapMouseButton(const SDL_MouseButtonEvent& mouseButtonEvent);

   public:
      static void initialize();
      static void handleSDLEvent(Rocket::Core::Context* context, const SDL_Event& event);
};

#endif
