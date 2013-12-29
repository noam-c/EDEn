/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Skill.h"
#include "ScriptEngine.h"
#include "Character.h"

Skill::Skill(Json::Value& skillNode) :
   Usable(skillNode)
{
}

UsableScript* Skill::createScript(ScriptEngine& scriptEngine)
{
   return scriptEngine.createSkillScript(*this);
}

bool Skill::use(ScriptEngine& scriptEngine, GameState::GameStateType gameStateType, Character* usingCharacter)
{
   if(usingCharacter != NULL && usingCharacter->hasSkill(getId()))
   {
      return Usable::use(scriptEngine, gameStateType, usingCharacter);
   }

   return false;
}
