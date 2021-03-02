/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "GameContext.h"
#include "ScriptEngine.h"

#include <RmlUi/Core/Factory.h>
#include "RocketScriptHandlerFactory.h"

GameContext::GameContext(ScriptEngine& scriptEngine) :
   m_scriptEngine(scriptEngine),
   m_metadata(m_scriptEngine)
{
   Rml::Core::Factory::RegisterEventListenerInstancer(new RocketScriptHandlerFactory(m_scriptEngine));
}

Metadata& GameContext::getMetadata()
{
   return m_metadata;
}

ScriptEngine& GameContext::getScriptEngine()
{
   return m_scriptEngine;
}
