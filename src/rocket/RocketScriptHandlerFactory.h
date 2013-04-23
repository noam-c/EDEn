#ifndef ROCKET_SCRIPT_HANDLER_FACTORY_H
#define ROCKET_SCRIPT_HANDLER_FACTORY_H

#include <Rocket/Core.h>

class GameContext;
class RocketScriptHandler;

class RocketScriptHandlerFactory : public Rocket::Core::EventListenerInstancer
{
   GameContext& gameContext;

   public:
      RocketScriptHandlerFactory(GameContext& gameContext);
      virtual ~RocketScriptHandlerFactory();

      virtual Rocket::Core::EventListener* InstanceEventListener(
            const Rocket::Core::String& value, Rocket::Core::Element* element);

      virtual void Release();
};

#endif
