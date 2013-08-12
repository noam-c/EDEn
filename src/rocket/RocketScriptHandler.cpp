#include "RocketScriptHandler.h"
#include "GameContext.h"

RocketScriptHandler::RocketScriptHandler(GameContext& gameContext, Rocket::Core::String scriptString) :
   m_gameContext(gameContext),
   m_scriptString(scriptString.CString())
{
}

RocketScriptHandler::~RocketScriptHandler()
{
}

void RocketScriptHandler::ProcessEvent(Rocket::Core::Event& event)
{
   Scheduler* scheduler = m_gameContext.getCurrentScheduler();
   m_gameContext.getScriptEngine().runScriptString(m_scriptString, *scheduler);
}

void RocketScriptHandler::OnDetach(Rocket::Core::Element* element)
{
   delete this;
}

