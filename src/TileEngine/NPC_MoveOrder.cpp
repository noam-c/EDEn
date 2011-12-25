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

#include "DebugUtils.h"
const int debugFlag = DEBUG_NPC;

NPC::MoveOrder::MoveOrder(NPC& npc, const Point2D& destination, Pathfinder& pathfinder)
: Order(npc), dst(destination), pathfinder(pathfinder)
{
}

void NPC::MoveOrder::updateDirection(MovementDirection newDirection)
{
   if(newDirection != npc.getDirection())
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
}

void NPC::MoveOrder::updateNextWaypoint(Point2D location, MovementDirection& direction)
{
   currLocation = location;
   nextLocation = path.front();
   
   // Set the direction based on where the next tile is relative to the current location.
   // For now, when the NPC must move diagonally, it will always face up or down
   if(location.y < nextLocation.y)
   {
      direction = DOWN;
   }
   else if(location.y > nextLocation.y)
   {
      direction = UP;
   }
   else if(location.x < nextLocation.x)
   {
      direction = RIGHT;
   }
   else if(location.x > nextLocation.x)
   {
      direction = LEFT;
   }
}

bool NPC::MoveOrder::perform(long timePassed)
{
   MovementDirection newDirection = npc.getDirection();
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
         DEBUG("Finding an ideal path from %d,%d to %d,%d", location.x, location.y, dst.x, dst.y);  
         path = pathfinder.findBestPath(location, dst);
         if(path.empty())
         {
            /** 
             * \todo This line will fire if the goal cannot possibly be reached.
                     It would be better to return an error to the coroutine that initiated this move.
             */
            DEBUG("Goal is currently unreachable!");  
            return true;
         }

         DEBUG("Found ideal path:");
         for(Pathfinder::Path::const_iterator iter = path.begin(); iter != path.end(); ++iter)
         {
            DEBUG("\t(%d,%d)", iter->x, iter->y);
         }

         // If the path exists, set the next waypoint
         updateNextWaypoint(location, newDirection);
         if(!pathfinder.beginMovement(&npc, location, nextLocation, 32, 32))
         {
            path = pathfinder.findReroutedPath(location, dst);
         }

         updateDirection(newDirection);
         return false;
      }
   }
   
   while(true)
   {
      const long stepDistance = std::max(abs(location.x - nextLocation.x), abs(location.y - nextLocation.y));

      if (distanceCovered < stepDistance)
      {
         // The NPC will not be able to make it to the next waypoint in this frame
         // Move towards the waypoint as much as possible.
         if(location.x < nextLocation.x)
         {
            location.x += distanceCovered;
            if(location.x > nextLocation.x) location.x = nextLocation.x;
         }
         else if(location.x > nextLocation.x)
         {
            location.x -= distanceCovered;
            if(location.x < nextLocation.x) location.x = nextLocation.x;
         }
         
         if(location.y < nextLocation.y)
         {
            location.y += distanceCovered;
            if(location.y > nextLocation.y) location.y = nextLocation.y;
         }
         else if(location.y > nextLocation.y)
         {
            location.y -= distanceCovered;
            if(location.y < nextLocation.y) location.y = nextLocation.y;
         }
         
         // Movement for this frame is finished
         break;
      }
      else
      {
         // The NPC can reach the next waypoint in this frame
         distanceCovered -= stepDistance;

         DEBUG("Reached waypoint %d,%d", nextLocation.x, nextLocation.y);
         pathfinder.endMovement(currLocation, nextLocation, 32, 32);

         // Update the current waypoint and dequeue it from the path
         location = nextLocation;
         path.pop_front();
         
         // If there are no more nodes, the NPC is finished for this frame
         // Unless something went wrong, the NPC reached the destination
         if(path.empty())
         {
            DEBUG("Path completed.");
            break;
         }
         
         // Get the next waypoint, and reroute if it is obstructed.
         updateNextWaypoint(location, newDirection);
         DEBUG("Next waypoint: %d,%d", nextLocation.x, nextLocation.y);

         bool canMove = pathfinder.beginMovement(&npc, currLocation, nextLocation, 32, 32);
         if(!canMove)
         {
            DEBUG("Path from %d,%d to %d,%d is obstructed. Rerouting...", currLocation.x, currLocation.y, nextLocation.x, nextLocation.y);
            path = pathfinder.findReroutedPath(location, dst);
            if(path.empty())
            {
               /** 
                * \todo This line will fire if the goal cannot be reached because of a dynamic obstruction.
                        It would be better to return an error to the coroutine that initiated this move so different coroutines can react in different ways.
                */
               DEBUG("Failed to reroute. Destination is completely obstructed.");
               break;
            }

            DEBUG("Found rerouted path:");
            for(Pathfinder::Path::const_iterator iter = path.begin(); iter != path.end(); ++iter)
            {
               DEBUG("\t(%d,%d)", iter->x, iter->y);
            }

            // If the path exists, set the next waypoint and finish the frame
            updateNextWaypoint(location, newDirection);
         }
      }
   }
   
   npc.setLocation(location);
   updateDirection(newDirection);
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