/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"
#include "NPC_Orders.h"
#include "GLInclude.h"
#include "TileEngine.h"
#include "Map.h"

NPC::MoveOrder::MoveOrder(NPC& npc, const Point2D& destination, const Map& map)
: Order(npc), dst(destination), currentLocation(npc.getLocation()), pathfinder(*map.getPathfinder())
{
}

bool NPC::MoveOrder::perform(long timePassed)
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
         return false;
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
   
   return false;
}

void NPC::MoveOrder::draw()
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