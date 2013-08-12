#include "RocketScriptHandlerFactory.h"
#include "RocketScriptHandler.h"

RocketScriptHandlerFactory::RocketScriptHandlerFactory(GameContext& gameContext) :
   m_gameContext(gameContext)
{
}

RocketScriptHandlerFactory::~RocketScriptHandlerFactory()
{
}

Rocket::Core::EventListener* RocketScriptHandlerFactory::InstanceEventListener(
      const Rocket::Core::String& value, Rocket::Core::Element* element)
{
   return new RocketScriptHandler(m_gameContext, value);
}

void RocketScriptHandlerFactory::Release()
{
   delete this;
}
