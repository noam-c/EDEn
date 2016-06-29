/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "RocketScriptHandlerFactory.h"
#include "RocketScriptHandler.h"

RocketScriptHandlerFactory::RocketScriptHandlerFactory(ScriptEngine& scriptEngine) :
   m_scriptEngine(scriptEngine)
{
}

Rocket::Core::EventListener* RocketScriptHandlerFactory::InstanceEventListener(
      const Rocket::Core::String& value, Rocket::Core::Element* element)
{
   return new RocketScriptHandler(m_scriptEngine, value);
}

void RocketScriptHandlerFactory::Release()
{
   delete this;
}
