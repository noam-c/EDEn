/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef ROCKET_SCRIPT_HANDLER_H
#define ROCKET_SCRIPT_HANDLER_H

#include <RmlUi/Core.h>

class ScriptEngine;

class RocketScriptHandler final : public Rml::Core::EventListener
{
   ScriptEngine& m_scriptEngine;
   const std::string m_scriptString;

   public:
      RocketScriptHandler(ScriptEngine& scriptEngine, Rml::Core::String scriptString);

      void ProcessEvent(Rml::Core::Event& event) override;

      void OnDetach(Rml::Core::Element* element) override;
};

#endif
