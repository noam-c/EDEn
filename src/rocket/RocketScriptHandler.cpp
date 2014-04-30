#include "RocketScriptHandler.h"
#include "ScriptEngine.h"

RocketScriptHandler::RocketScriptHandler(ScriptEngine& scriptEngine, Rocket::Core::String scriptString) :
   m_scriptEngine(scriptEngine),
   m_scriptString(scriptString.CString())
{
}

void RocketScriptHandler::ProcessEvent(Rocket::Core::Event& event)
{
   m_scriptEngine.runScriptString(m_scriptString);
}

void RocketScriptHandler::OnDetach(Rocket::Core::Element* element)
{
   delete this;
}

