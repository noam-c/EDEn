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

Rml::Core::EventListener* RocketScriptHandlerFactory::InstanceEventListener(
      const Rml::Core::String& value, Rml::Core::Element* element)
{
   return new RocketScriptHandler(m_scriptEngine, value);
}