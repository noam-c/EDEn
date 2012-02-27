/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"

#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"
#include "Map.h"
#include "EntityGrid.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

NPC::NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name, const std::string& sheetName, EntityGrid& entityGrid,
                       const std::string& regionName,
                       int x, int y) : Actor(name, sheetName, entityGrid, x, y, 0.1f, DOWN)
{
   npcThread = engine.getNPCScript(this, regionName, entityGrid.getMapData()->getName(), name);
   scheduler.start(npcThread);
   DEBUG("NPC %s has a Thread with ID %d", name.c_str(), npcThread->getId());
}

NPC::~NPC()
{
   npcThread->finish();
}

void NPC::activate()
{
   flushOrders();
   npcThread->activate();
}

