/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_LISTENER_H
#define ROCKET_LISTENER_H

#include <RmlUi/Core.h>
#include <functional>

class RocketListener final : public Rml::Core::EventListener
{
   Rml::Core::Element* m_element;
   Rml::Core::String m_eventType;
   bool m_capture;
   std::function<void(Rml::Core::Event&)> m_processFunction;

   public:
      RocketListener(Rml::Core::Element* element, Rml::Core::String eventType, bool capture, std::function<void(Rml::Core::Event&)> processFunction);

      RocketListener(const RocketListener& listener);

      RocketListener(RocketListener&& listener);

      ~RocketListener() override;

      void ProcessEvent(Rml::Core::Event& event) override;
};

#endif
