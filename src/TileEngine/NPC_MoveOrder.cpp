/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "NPC.h"
#include "NPC_Orders.h"
#include "GLInclude.h"
#include "TileEngine.h"
#include "Map.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_NPC;

NPC::MoveOrder::MoveOrder(NPC& npc, const Point2D& destination, Pathfinder& pathfinder)
: Order(npc), pathInitialized(false), movementBegun(false), dst(destination), pathfinder(pathfinder)
{
}

NPC::MoveOrder::~MoveOrder()
{
   if(movementBegun)
   {
      pathfinder.abortMovement(&npc, lastWaypoint, nextWaypoint, npc.getLocation(), 32, 32);
   }
}

void NPC::MoveOrder::updateDirection(MovementDirection newDirection, bool moving)
{
   npc.setDirection(newDirection);
   if(moving)
   {
      npc.setAnimation(WALKING_PREFIX);
   }
   else
   {
      npc.setFrame(STANDING_PREFIX);
   }
}

void NPC::MoveOrder::updateNextWaypoint(Point2D location, MovementDirection& direction)
{
   lastWaypoint = location;
   nextWaypoint = path.front();
   
   // Set the direction based on where the next tile is relative to the current location.
   // For now, when the NPC must move diagonally, it will always face up or down
   if(location.y < nextWaypoint.y)
   {
      direction = DOWN;
   }
   else if(location.y > nextWaypoint.y)
   {
      direction = UP;
   }
   else if(location.x < nextWaypoint.x)
   {
      direction = RIGHT;
   }
   else if(location.x > nextWaypoint.x)
   {
      direction = LEFT;
   }
}

bool NPC::MoveOrder::perform(long timePassed)
{
   Point2D location = npc.getLocation();
   MovementDirection newDirection = npc.getDirection();
   const float vel = npc.getMovementSpeed();
   long distanceCovered = timePassed * vel;
   
   // If first run, get the best computed path (RFW), end frame
   // loop infinitely
   //      if there is no next vertex
   //          if NPC is at the destination
   //             end task
   //          else
   //             create a rerouted path (A*)
   //             end frame
   //          
   //      face next vertex
   //      if vertex isn't yet acquired
   //          try acquire vertex
   //          if acquire failed
   //             create a rerouted path (A*)
   //             end frame
   //
   //      if vertex is within step
   //          move to vertex
   //          free previous vertex
   //      else
   //          move as close as possible to vertex
   //          break
   //
   // end frame

   if(!pathInitialized)
   {
      DEBUG("Finding an ideal path from %d,%d to %d,%d", location.x, location.y, dst.x, dst.y);  
      path = pathfinder.findBestPath(location, dst);
      pathInitialized = true;
      return false;
   }

   for(;;)
   {
      if(path.empty())
      {
         updateDirection(npc.getDirection(), false);
         npc.setLocation(location);
         if(location != dst)
         {
            path = pathfinder.findReroutedPath(location, dst, 32, 32);
            return false;
         }

         return true;
      }
      
      if(!movementBegun)
      {
         movementBegun = pathfinder.beginMovement(&npc, location, path.front(), 32, 32);
         if(!movementBegun)
         {
            path = pathfinder.findReroutedPath(location, dst, 32, 32);
            updateDirection(npc.getDirection(), false);
            npc.setLocation(location);
            return false;
         }

         DEBUG("Next waypoint: %d,%d", nextWaypoint.x, nextWaypoint.y);
         updateNextWaypoint(location, newDirection);
         updateDirection(newDirection, true);
      }
      
      const long stepDistance = std::max(abs(location.x - nextWaypoint.x), abs(location.y - nextWaypoint.y));
      
      if (distanceCovered < stepDistance)
      {
         // The NPC will not be able to make it to the next waypoint in this frame
         // Move towards the waypoint as much as possible.
         if(location.x < nextWaypoint.x)
         {
            location.x += distanceCovered;
            if(location.x > nextWaypoint.x) location.x = nextWaypoint.x;
         }
         else if(location.x > nextWaypoint.x)
         {
            location.x -= distanceCovered;
            if(location.x < nextWaypoint.x) location.x = nextWaypoint.x;
         }
         
         if(location.y < nextWaypoint.y)
         {
            location.y += distanceCovered;
            if(location.y > nextWaypoint.y) location.y = nextWaypoint.y;
         }
         else if(location.y > nextWaypoint.y)
         {
            location.y -= distanceCovered;
            if(location.y < nextWaypoint.y) location.y = nextWaypoint.y;
         }
         
         // Movement for this frame is finished
         npc.setLocation(location);
         return false;
      }
      
      // The NPC can reach the next waypoint in this frame
      distanceCovered -= stepDistance;
      
      DEBUG("Reached waypoint %d,%d", nextWaypoint.x, nextWaypoint.y);
      pathfinder.endMovement(&npc, lastWaypoint, nextWaypoint, 32, 32);
      movementBegun = false;

      // Update the current waypoint and dequeue it from the path
      location = nextWaypoint;
      path.pop_front();
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
      Point2D point(iter->x + TileEngine::TILE_SIZE / 2, iter->y + TileEngine::TILE_SIZE / 2);
      glVertex3d(point.x, point.y, 0);
   }
   glEnd();
   
   glEnable(GL_TEXTURE_2D);
}
