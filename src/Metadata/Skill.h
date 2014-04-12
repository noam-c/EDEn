/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef SKILL_H
#define SKILL_H

#include "Usable.h"

class Skill : public Usable
{
   static const char* MASTERY_ATTRIBUTE;

   unsigned int m_masteryThreshold;

   protected:
      std::shared_ptr<UsableScript> createScript(ScriptEngine& scriptEngine);

   public:
      /**
       * Constructor.
       *
       * @param skillNode The JSON node containing the skill metadata to load.
       */
      Skill(Json::Value& skillNode);
   
      unsigned int getAdeptitudeThreshold();
      unsigned int getMasteryThreshold();

      bool use(ScriptEngine& scriptEngine, GameStateType gameStateType, Character* usingCharacter);
};

#endif
