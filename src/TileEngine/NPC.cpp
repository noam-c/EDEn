/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"

#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"
#include "Map.h"
#include "EntityGrid.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_ACTOR

NPC::NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name, const MovementDirection direction, const std::string& sheetName,
            messaging::MessagePipe& messagePipe, EntityGrid& entityGrid, const std::string& regionName, const shapes::Point2D& location, const shapes::Size& size) :
   Actor(name, messagePipe, entityGrid, location, size, 0.1f, direction)
{
   m_coroutine = engine.createNPCCoroutine(this, regionName, entityGrid.getMapName());
   scheduler.start(m_coroutine);
   setSpritesheet(sheetName);
   DEBUG("NPC %s has a Coroutine with ID %d", name.c_str(), m_coroutine->getId());
}

NPC::~NPC()
{
   m_coroutine->finish();
}

void NPC::activate()
{
   flushOrders();
   m_coroutine->activate();
}

