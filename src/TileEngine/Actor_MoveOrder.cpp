/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Actor.h"
#include "Actor_Orders.h"
#include "SDL_opengl.h"
#include "TileEngine.h"
#include "Map.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_NPC;

//#define DRAW_PATH

Actor::MoveOrder::MoveOrder(Actor& actor, const shapes::Point2D& destination, EntityGrid& entityGrid)
: Order(actor), pathInitialized(false), movementBegun(false), dst(destination), entityGrid(entityGrid), cumulativeDistanceCovered(0)
{	
}

Actor::MoveOrder::~MoveOrder()
{
   if(movementBegun)
   {
      entityGrid.abortMovement(&actor, lastWaypoint, nextWaypoint);
   }
}

void Actor::MoveOrder::updateDirection(MovementDirection newDirection, bool moving)
{
   actor.setDirection(newDirection);
   if(moving)
   {
      actor.setAnimation(WALKING_PREFIX);
   }
   else
   {
      actor.setFrame(STANDING_PREFIX);
   }
}

void Actor::MoveOrder::updateNextWaypoint(shapes::Point2D location, MovementDirection& direction)
{
   lastWaypoint = location;
   nextWaypoint = path.front();
   
   // Set the direction based on where the next tile is relative to the current location.
   // For now, when the Actor must move diagonally, it will always face up or down
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

bool Actor::MoveOrder::perform(long timePassed)
{
   shapes::Point2D location = actor.getLocation();
   MovementDirection newDirection = actor.getDirection();
   const float vel = actor.getMovementSpeed();
   cumulativeDistanceCovered +=timePassed * vel;   
   long distanceCovered = 0;
   if(cumulativeDistanceCovered > 1.0)
   {
	   distanceCovered = floor(cumulativeDistanceCovered);
	   cumulativeDistanceCovered -= distanceCovered;
   }
   // If first run, get the best computed path (RFW), end frame
   // loop infinitely
   //      if there is no next vertex
   //          if Actor is at the destination
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
      path = entityGrid.findBestPath(location, dst);
      pathInitialized = true;
      return false;
   }

   for(;;)
   {
      if(path.empty())
      {
         updateDirection(actor.getDirection(), false);
         actor.setLocation(location);
         if(location != dst)
         {
            path = entityGrid.findReroutedPath(location, dst, actor.getWidth(), actor.getHeight());
            return false;
         }

         return true;
      }
      
      if(!movementBegun)
      {
         movementBegun = entityGrid.beginMovement(&actor, path.front());
         if(!movementBegun)
         {
            path = entityGrid.findReroutedPath(location, dst, actor.getWidth(), actor.getHeight());
            updateDirection(actor.getDirection(), false);
            actor.setLocation(location);
            return false;
         }

         DEBUG("Next waypoint: %d,%d", nextWaypoint.x, nextWaypoint.y);
         updateNextWaypoint(location, newDirection);
         updateDirection(newDirection, true);
      }
      
      const long stepDistance = std::max(abs(location.x - nextWaypoint.x), abs(location.y - nextWaypoint.y));
      
      if (distanceCovered < stepDistance)
      {
         // The Actor will not be able to make it to the next waypoint in this frame
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
         actor.setLocation(location);
         return false;
      }
      
      // The Actor can reach the next waypoint in this frame
      distanceCovered -= stepDistance;
      
      DEBUG("Reached waypoint %d,%d", nextWaypoint.x, nextWaypoint.y);
      entityGrid.endMovement(&actor, lastWaypoint, nextWaypoint);
      movementBegun = false;

      // Update the current waypoint and dequeue it from the path
      location = nextWaypoint;
      path.pop_front();
   }

   return false;
}

void Actor::MoveOrder::draw()
{
#if DRAW_PATH
   if(path.empty()) return;

   glDisable(GL_TEXTURE_2D);
   glColor3f(1.0f, 0.0f, 0.0f);
   glBegin(GL_LINE_STRIP);
   for(EntityGrid::Path::const_iterator iter = path.begin(); iter != path.end(); ++iter)
   {
      shapes::Point2D point(iter->x + TileEngine::TILE_SIZE / 2, iter->y + TileEngine::TILE_SIZE / 2);
      glVertex3d(point.x, point.y, 0);
   }
   glEnd();
   
   glEnable(GL_TEXTURE_2D);
#endif
}
