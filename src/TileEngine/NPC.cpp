/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"

#include <algorithm>
#include "stdlib.h"

#include "NPC_Orders.h"
#include "ScriptEngine.h"
#include "NPCScript.h"
#include "Scheduler.h"
#include "Sprite.h"
#include "Map.h"
#include "TileEngine.h"
#include "Pathfinder.h"
#include "GLInclude.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_NPC;

NPC::NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name, Spritesheet* sheet, Pathfinder& pathfinder,
                       const std::string& regionName,
                       int x, int y) : name(name), pathfinder(pathfinder), pixelLoc(x, y), currDirection(DOWN), movementSpeed(0.1f)
{
   npcThread = engine.getNPCScript(this, regionName, pathfinder.getMapData()->getName(), name);
   scheduler.start(npcThread);
   DEBUG("NPC %s has a Thread with ID %d", name.c_str(), npcThread->getId());

   sprite = new Sprite(sheet);
}

NPC::~NPC()
{
   delete sprite;
   npcThread->finish();
}

std::string NPC::getName() const
{
   return name;
}

void NPC::step(long timePassed)
{
   sprite->step(timePassed);

   if(!isIdle())
   {
      Order* currentOrder = orders.front();
      if(currentOrder->perform(timePassed))
      {
         orders.pop();
         delete currentOrder;
      }
   }
}

bool NPC::isIdle() const
{
   return orders.empty();
}

void NPC::activate()
{
   npcThread->activate();
}

void NPC::move(int x, int y)
{
   Point2D dst(x, y);
   orders.push(new MoveOrder(*this, dst, pathfinder));
}

void NPC::setSpritesheet(Spritesheet* sheet)
{
   sprite->setSheet(sheet);
}

void NPC::setFrame(const std::string& frameName)
{
   sprite->setFrame(frameName, currDirection);
}

void NPC::setAnimation(const std::string& animationName)
{
   sprite->setAnimation(animationName, currDirection);
}

void NPC::setLocation(Point2D location)
{
   pixelLoc = location;
}

Point2D NPC::getLocation() const
{
   return pixelLoc;
}

void NPC::setDirection(MovementDirection direction)
{
   currDirection = direction;
}

MovementDirection NPC::getDirection() const
{
   return currDirection;
}

void NPC::setMovementSpeed(float speed)
{
   movementSpeed = speed;
}

float NPC::getMovementSpeed() const
{
   return movementSpeed;
}

void NPC::draw()
{
   if(sprite)
   {
      sprite->draw(pixelLoc.x, pixelLoc.y + TileEngine::TILE_SIZE);
   }
   
   if(!orders.empty())
   {
      orders.front()->draw();
   }
}

