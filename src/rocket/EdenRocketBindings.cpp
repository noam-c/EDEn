/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "EdenRocketBindings.h"

void EdenRocketBindings::bindAction(Rocket::Core::ElementDocument* document, const char* id, const char* eventType, std::function<void(Rocket::Core::Event&)> handler, bool capture)
{
   Rocket::Core::Element* element = document->GetElementById(id);
   if(element != nullptr)
   {
      bindAction(element, eventType, handler, capture);
   }
}

void EdenRocketBindings::bindAction(Rocket::Core::Element* element, const char* eventType, std::function<void(Rocket::Core::Event&)> handler, bool capture)
{
   m_listeners.emplace_back(element, eventType, capture, handler);
}
