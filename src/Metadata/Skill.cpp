/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Skill.h"
#include "GameContext.h"

Skill::Skill(Json::Value& skillNode) :
   Usable(skillNode)
{
}

UsableScript* Skill::createScript(GameContext& gameContext)
{
   return gameContext.getScriptEngine().createSkillScript(*this);
}
