/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"

#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"
#include "Map.h"
#include "EntityGrid.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

NPC::NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name, const MovementDirection direction, const std::string& sheetName,
            messaging::MessagePipe& messagePipe, EntityGrid& entityGrid, const std::string& regionName, const shapes::Point2D& location, const shapes::Size& size) :
   Actor(name, messagePipe, entityGrid, location, size, 0.1f, direction)
{
   npcCoroutine = engine.createNPCCoroutine(this, regionName, entityGrid.getMapData()->getName());
   scheduler.start(npcCoroutine);
   setSpritesheet(sheetName);
   DEBUG("NPC %s has a Coroutine with ID %d", name.c_str(), npcCoroutine->getId());
}

NPC::~NPC()
{
   npcCoroutine->finish();
}

void NPC::activate()
{
   flushOrders();
   npcCoroutine->activate();
}

