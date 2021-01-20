/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "RocketScriptHandler.h"
#include "ScriptEngine.h"

RocketScriptHandler::RocketScriptHandler(ScriptEngine& scriptEngine, Rml::Core::String scriptString) :
   m_scriptEngine(scriptEngine),
   m_scriptString(scriptString.c_str())
{
}

void RocketScriptHandler::ProcessEvent(Rml::Core::Event& event)
{
   m_scriptEngine.runScriptString(m_scriptString);
}

void RocketScriptHandler::OnDetach(Rml::Core::Element* element)
{
   delete this;
}

