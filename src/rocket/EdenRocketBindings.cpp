/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "EdenRocketBindings.h"

void EdenRocketBindings::bindAction(Rml::Core::Element* container, const char* id, const char* eventType, std::function<void(Rml::Core::Event&)> handler, bool capture)
{
   auto element = container->GetElementById(id);
   if(element != nullptr)
   {
      bindAction(element, eventType, handler, capture);
   }
}

void EdenRocketBindings::bindAction(Rml::Core::Element* element, const char* eventType, std::function<void(Rml::Core::Event&)> handler, bool capture)
{
   m_listeners.emplace_back(element, eventType, capture, handler);
}
