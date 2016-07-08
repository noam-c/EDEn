/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "LuaCombatActor.h"
#include "ScriptUtilities.h"
#include "ScriptEngine.h"
#include "CombatActor.h"

static luaL_Reg combatActorMetatable[] =
{
   { nullptr, nullptr }
};

void luaopen_CombatActor(lua_State* luaVM)
{
   luaW_register<CombatActor>(luaVM, "CombatActor", nullptr, combatActorMetatable, nullptr, nullptr);
}
