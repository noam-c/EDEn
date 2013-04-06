/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "EdenRocketSystemInterface.h"
#include "SDL.h"
#include <Rocket/Core.h>

#include "DebugUtils.h"
const int debugFlag = DEBUG_ROCKET;

float EdenRocketSystemInterface::GetElapsedTime()
{
   return SDL_GetTicks();
}

bool EdenRocketSystemInterface::LogMessage(Rocket::Core::Log::Type type, const Rocket::Core::String& message)
{
   DEBUG("%s", message.CString());
   return true;
}
