/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_SCRIPT_HANDLER_FACTORY_H
#define ROCKET_SCRIPT_HANDLER_FACTORY_H

#include <RmlUi/Core.h>

class ScriptEngine;
class RocketScriptHandler;

class RocketScriptHandlerFactory final : public Rml::Core::EventListenerInstancer
{
   ScriptEngine& m_scriptEngine;

   public:
      RocketScriptHandlerFactory(ScriptEngine& scriptEngine);

      Rml::Core::EventListener* InstanceEventListener(
            const Rml::Core::String& value, Rml::Core::Element* element) override;
};

#endif
