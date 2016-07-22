/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_SCRIPT_HANDLER_H
#define ROCKET_SCRIPT_HANDLER_H

#include <Rocket/Core.h>

class ScriptEngine;

class RocketScriptHandler final : public Rocket::Core::EventListener
{
   ScriptEngine& m_scriptEngine;
   const std::string m_scriptString;

   public:
      RocketScriptHandler(ScriptEngine& scriptEngine, Rocket::Core::String scriptString);

      virtual void ProcessEvent(Rocket::Core::Event& event) override;

      virtual void OnDetach(Rocket::Core::Element* element) override;
};

#endif
