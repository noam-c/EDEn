/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Skill.h"
#include "json.h"
#include "ScriptEngine.h"
#include "Character.h"

const char* Skill::MASTERY_ATTRIBUTE = "mastery";

Skill::Skill(Json::Value& skillNode) :
   Usable(skillNode),
   m_masteryThreshold(skillNode[Skill::MASTERY_ATTRIBUTE].asUInt())
{
}

std::shared_ptr<UsableScript> Skill::createScript(ScriptEngine& scriptEngine)
{
   return scriptEngine.createSkillScript(*this);
}

bool Skill::use(ScriptEngine& scriptEngine, GameStateType gameStateType, Character* usingCharacter)
{
   if(usingCharacter != nullptr && usingCharacter->hasSkill(getId()))
   {
      return Usable::use(scriptEngine, gameStateType, usingCharacter);
   }

   return false;
}

unsigned int Skill::getAdeptitudeThreshold() const
{
   return m_masteryThreshold / 2;
}

unsigned int Skill::getMasteryThreshold() const
{
   return m_masteryThreshold;
}
