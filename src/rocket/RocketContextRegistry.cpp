/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "RocketContextRegistry.h"
#include <Rocket/Core.h>
#include <algorithm>

int RocketContextRegistry::GetEventClasses()
{
   return EVT_BASIC;
}

void RocketContextRegistry::OnContextCreate(Rocket::Core::Context* context)
{
   activeRocketContexts.push_back(context);
}

void RocketContextRegistry::OnContextDestroy(Rocket::Core::Context* context)
{
   // Search for the context to remove from back to front, since contexts are usually
   // destroyed in reverse order of creation.
   std::remove(activeRocketContexts.rbegin(), activeRocketContexts.rend(), context);
}

const std::vector<Rocket::Core::Context*>& RocketContextRegistry::getActiveContexts() const
{
   return activeRocketContexts;
}