/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "EdenRocketSystemInterface.h"
#include "SDL.h"
#include <RmlUi/Core.h>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ROCKET

double EdenRocketSystemInterface::GetElapsedTime()
{
   return static_cast<double>(SDL_GetTicks()) / 1000.0;
}

bool EdenRocketSystemInterface::LogMessage(Rml::Core::Log::Type type, const Rml::Core::String& message)
{
   DEBUG("%s", message.c_str());
   return true;
}
