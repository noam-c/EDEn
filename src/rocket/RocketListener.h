/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_LISTENER_H
#define ROCKET_LISTENER_H

#include <Rocket/Core.h>
#include <functional>

class RocketListener final : public Rocket::Core::EventListener
{
   Rocket::Core::Element* m_element;
   Rocket::Core::String m_eventType;
   bool m_capture;
   std::function<void(Rocket::Core::Event&)> m_processFunction;

   public:
      RocketListener(Rocket::Core::Element* element, Rocket::Core::String eventType, bool capture, std::function<void(Rocket::Core::Event&)> processFunction);

      RocketListener(const RocketListener& listener);

      RocketListener(RocketListener&& listener);

      ~RocketListener() override;

      void ProcessEvent(Rocket::Core::Event& event) override;
};

#endif
