/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "RocketListener.h"

RocketListener::RocketListener(Rocket::Core::Element* element, Rocket::Core::String eventType, bool capture, std::function<void(Rocket::Core::Event&)> processFunction) :
   m_element(element),
   m_eventType(eventType),
   m_capture(capture),
   m_processFunction(processFunction)
{
   m_element->AddEventListener(m_eventType, this, m_capture);
}

RocketListener::~RocketListener()
{
   m_element->RemoveEventListener(m_eventType, this, m_capture);
}

RocketListener::RocketListener(const RocketListener& listener) : RocketListener::RocketListener(listener.m_element, listener.m_eventType, listener.m_capture, listener.m_processFunction)
{
}

RocketListener::RocketListener(RocketListener&& listener) : RocketListener::RocketListener(listener.m_element, listener.m_eventType, listener.m_capture, listener.m_processFunction)
{
}

void RocketListener::ProcessEvent(Rocket::Core::Event& event)
{
   m_processFunction(event);
}
