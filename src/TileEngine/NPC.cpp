/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"

#include <algorithm>
#include "stdlib.h"

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

const std::string WALKING_PREFIX = "walk_";
const std::string STANDING_PREFIX = "stand_";

class NPC::NPCTask
{
   protected:
      NPC& npc;
      NPCTask(NPC& npc) : npc(npc) {}
      
   public:
      virtual bool perform(long timePassed) = 0;
      virtual void draw() {}
      virtual ~NPCTask() {}
};

class NPC::StandTask : public NPC::NPCTask
{
   public:
      bool perform(long timePassed);
};

class NPC::MoveTask : public NPC::NPCTask
{
   const Point2D dst;
   Point2D currentLocation;
   Pathfinder& pathfinder;
   Pathfinder::Path path;

   public:
      MoveTask(NPC& npc, const Point2D& destination, const Map& map);
      bool perform(long timePassed);
      void draw();
};

NPC::NPC(ScriptEngine& engine, Scheduler& scheduler, const std::string& name, Spritesheet* sheet, const Map& map,
                       const std::string& regionName,
                       int x, int y) : name(name), map(map), pixelLoc(x, y), movementSpeed(0.1f)
{
   npcThread = engine.getNPCScript(this, regionName, map.getName(), name);
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

bool NPC::StandTask::perform(long timePassed)
{
   switch(npc.getDirection())
   {
      case LEFT:
      {
         npc.setFrame(STANDING_PREFIX + "left");
         break;
      }
      case RIGHT:
      {
         npc.setFrame(STANDING_PREFIX + "right");
         break;
      }
      case UP:
      {
         npc.setFrame(STANDING_PREFIX + "up");
         break;
      }
      case DOWN:
      {
         npc.setFrame(STANDING_PREFIX + "down");
         break;
      }
      default:
      {
         break;
      }
   }
   
   return true;
}

void NPC::step(long timePassed)
{
   sprite->step(timePassed);

   if(!isIdle())
   {
      NPCTask* currentTask = tasks.front();
      if(currentTask->perform(timePassed))
      {
         tasks.pop();
         delete currentTask;
      }
   }
}

bool NPC::isIdle() const
{
   return tasks.empty();
}

void NPC::activate()
{
   npcThread->activate();
}

void NPC::move(int x, int y)
{
   Point2D dst(x, y);
   
   MoveTask* task = new MoveTask(*this, dst, map);
   tasks.push(task);
}

void NPC::setSpritesheet(Spritesheet* sheet)
{
   sprite->setSheet(sheet);
}

void NPC::setFrame(const std::string& frameName)
{
   sprite->setFrame(frameName);
}

void NPC::setAnimation(const std::string& animationName)
{
   sprite->setAnimation(animationName);
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
   
   if(!tasks.empty())
   {
      tasks.front()->draw();
   }
}

NPC::MoveTask::MoveTask(NPC& npc, const Point2D& destination, const Map& map)
: NPCTask(npc), dst(destination), currentLocation(npc.getLocation()), pathfinder(*map.getPathfinder())
{
}

bool NPC::MoveTask::perform(long timePassed)
{
   MovementDirection currDirection = npc.getDirection();
   MovementDirection newDirection = currDirection;
   Point2D location = npc.getLocation();

   float vel = npc.getMovementSpeed();
   long distanceCovered = timePassed * vel;
   if(path.empty())
   {
      if(location == dst)
      {
         return true;
      }
      else
      {
         path = pathfinder.findPath(location.x, location.y, dst.x, dst.y);
      }
   }

   while(!path.empty())
   {
      Point2D newCoords = path.front();

      // Set the direction based on where the next tile is relative to the current location.
      // For now, when the NPC must move diagonally, it will always face up or down
      if(location.y < newCoords.y)
      {
         newDirection = DOWN;
      }
      else if(location.y > newCoords.y)
      {
         newDirection = UP;
      }
      else if(location.x < newCoords.x)
      {
         newDirection = RIGHT;
      }
      else if(location.x > newCoords.x)
      {
         newDirection = LEFT;
      } 

      const long stepDistance = std::max(abs(location.x - newCoords.x), abs(location.y - newCoords.y));
      
      if (distanceCovered < stepDistance)
      {
         if(location.x < newCoords.x)
         {
            location.x += distanceCovered;
            if(location.x > newCoords.x) location.x = newCoords.x;
         }
         else if(location.x > newCoords.x)
         {
            location.x -= distanceCovered;
            if(location.x < newCoords.x) location.x = newCoords.x;
         }

         if(location.y < newCoords.y)
         {
            location.y += distanceCovered;
            if(location.y > newCoords.y) location.y = newCoords.y;
         }
         else if(location.y > newCoords.y)
         {
            location.y -= distanceCovered;
            if(location.y < newCoords.y) location.y = newCoords.y;
         }

         break;
      }

      distanceCovered -= stepDistance;
      location = newCoords;
      currentLocation = newCoords;
      path.pop_front();
   }
   
   npc.setLocation(location);
   
   if(newDirection != currDirection)
   {
      npc.setDirection(newDirection);
      switch(newDirection)
      {
         case LEFT:
         {
            npc.setAnimation(WALKING_PREFIX + "left");
            break;
         }
         case RIGHT:
         {
            npc.setAnimation(WALKING_PREFIX + "right");
            break;
         }
         case UP:
         {
            npc.setAnimation(WALKING_PREFIX + "up");
            break;
         }
         case DOWN:
         {
            npc.setAnimation(WALKING_PREFIX + "down");
            break;
         }
         default:
         {
            break;
         }
      }
   }

   return path.empty();
}

void NPC::MoveTask::draw()
{
   glDisable(GL_TEXTURE_2D);
   glColor3f(1.0f, 0.0f, 0.0f);
   glBegin(GL_LINE_STRIP);
   for(Pathfinder::Path::const_iterator iter = path.begin(); iter != path.end(); ++iter)
   {
      Point2D point = *iter;
      point.x += TileEngine::TILE_SIZE / 2;

      point.y += TileEngine::TILE_SIZE / 2;

      glVertex3d(point.x, point.y, 0);
   }
   glEnd();
   
   glEnable(GL_TEXTURE_2D);
}
