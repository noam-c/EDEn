/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_SCRIPT_HANDLER_FACTORY_H
#define ROCKET_SCRIPT_HANDLER_FACTORY_H

#include <Rocket/Core.h>

class ScriptEngine;
class RocketScriptHandler;

class RocketScriptHandlerFactory final : public Rocket::Core::EventListenerInstancer
{
   ScriptEngine& m_scriptEngine;

   public:
      RocketScriptHandlerFactory(ScriptEngine& scriptEngine);

      Rocket::Core::EventListener* InstanceEventListener(
            const Rocket::Core::String& value, Rocket::Core::Element* element) override;

      void Release() override;
};

#endif
