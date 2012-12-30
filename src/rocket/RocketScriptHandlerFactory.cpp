#include "RocketScriptHandlerFactory.h"
#include "RocketScriptHandler.h"

RocketScriptHandlerFactory::RocketScriptHandlerFactory(GameContext& gameContext) :
   gameContext(gameContext)
{
}

RocketScriptHandlerFactory::~RocketScriptHandlerFactory()
{
}

Rocket::Core::EventListener* RocketScriptHandlerFactory::InstanceEventListener(
      const Rocket::Core::String& value)
{
   return new RocketScriptHandler(gameContext, value);
}

void RocketScriptHandlerFactory::Release()
{
   delete this;
}
