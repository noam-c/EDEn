#include "RocketScriptHandler.h"
#include "GameContext.h"

RocketScriptHandler::RocketScriptHandler(GameContext& gameContext, Rocket::Core::String scriptString) :
   gameContext(gameContext),
   scriptString(scriptString.CString())
{
}

RocketScriptHandler::~RocketScriptHandler()
{
}

void RocketScriptHandler::ProcessEvent(Rocket::Core::Event& event)
{
   Scheduler* scheduler = gameContext.getCurrentScheduler();
   gameContext.getScriptEngine().runScriptString(scriptString, *scheduler);
}

void RocketScriptHandler::OnDetach(Rocket::Core::Element* element)
{
   delete this;
}

