#ifndef ROCKET_SCRIPT_HANDLER_H
#define ROCKET_SCRIPT_HANDLER_H

#include <Rocket/Core.h>

class GameContext;

class RocketScriptHandler : public Rocket::Core::EventListener
{
   GameContext& gameContext;
   const std::string scriptString;

   public:
      RocketScriptHandler(GameContext& gameContext, Rocket::Core::String scriptString);

      virtual ~RocketScriptHandler();

      virtual void ProcessEvent(Rocket::Core::Event& event);

      virtual void OnDetach(Rocket::Core::Element* element);
};

#endif
