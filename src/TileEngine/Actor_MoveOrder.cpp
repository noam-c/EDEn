/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Actor.h"

#include <math.h>
#include "SDL_opengl.h"

#include "Actor_Orders.h"
#include "Map.h"
#include "Direction.h"
#include "TileEngine.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_ACTOR

// Define as 1 to draw the NPC's projected path to the screen
#define DRAW_PATH 0

Actor::MoveOrder::MoveOrder(Actor& actor, const std::shared_ptr<Task>& task, const geometry::Point2D& destination, EntityGrid& entityGrid) :
   Order(actor),
   m_dst(destination),
   m_task(task),
   m_entityGrid(entityGrid)
{
}

Actor::MoveOrder::~MoveOrder()
{
   if(m_movementBegun)
   {
      m_entityGrid.abortMovement(&m_actor, m_lastWaypoint, m_nextWaypoint);
   }
}

void Actor::MoveOrder::updateDirection(geometry::Direction newDirection, bool moving)
{
   m_actor.setDirection(newDirection);
   if(moving)
   {
      m_actor.setAnimation(Actor::DEFAULT_WALKING_PREFIX);
   }
   else
   {
      m_actor.setFrame(Actor::DEFAULT_STANDING_PREFIX);
   }
}

void Actor::MoveOrder::updateNextWaypoint(geometry::Point2D location, geometry::Direction& direction)
{
   m_lastWaypoint = location;
   m_nextWaypoint = m_path.front();

   // Set the direction based on where the next tile is relative to the current location.
   // For now, when the Actor must move diagonally, it will always face up or down
   if(location.y < m_nextWaypoint.y)
   {
      direction = geometry::Direction::DOWN;
   }
   else if(location.y > m_nextWaypoint.y)
   {
      direction = geometry::Direction::UP;
   }
   else if(location.x < m_nextWaypoint.x)
   {
      direction = geometry::Direction::RIGHT;
   }
   else if(location.x > m_nextWaypoint.x)
   {
      direction = geometry::Direction::LEFT;
   }
}

bool Actor::MoveOrder::perform(long timePassed)
{
   geometry::Point2D location = m_actor.getLocation();
   geometry::Direction newDirection = m_actor.getDirection();
   const float vel = m_actor.getMovementSpeed();
   m_cumulativeDistanceCovered +=timePassed * vel;
   long distanceCovered = 0;
   if(m_cumulativeDistanceCovered > 1.0)
   {
	   distanceCovered = floor(m_cumulativeDistanceCovered);
	   m_cumulativeDistanceCovered -= distanceCovered;
   }
   // If first run, get the best pre-computed path (RFW), end frame
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

   if(!m_pathInitialized)
   {
      DEBUG("Finding an ideal path from %d,%d to %d,%d", location.x, location.y, m_dst.x, m_dst.y);
      m_path = m_entityGrid.findBestPath(location, m_dst, m_actor.getSize());
      if(m_path.empty())
      {
         // If this path is blocked, then there must be a permanent obstruction.
         return true;
      }

      // If a path was found, note that we have a path and end the frame
      m_pathInitialized = true;
      return false;
   }

   for(;;)
   {
      if(m_path.empty())
      {
         updateDirection(m_actor.getDirection(), false);
         m_actor.setLocation(location);
         if(location != m_dst)
         {
            m_path = m_entityGrid.findReroutedPath(location, m_dst, m_actor.getSize());
            return false;
         }

         if(m_task)
         {
            m_task->complete();
         }

         return true;
      }

      if(!m_movementBegun)
      {
         m_movementBegun = m_entityGrid.beginMovement(&m_actor, m_path.front());
         if(!m_movementBegun)
         {
            m_path = m_entityGrid.findReroutedPath(location, m_dst, m_actor.getSize());
            updateDirection(m_actor.getDirection(), false);
            m_actor.setLocation(location);
            return false;
         }

         DEBUG("Next waypoint: %d,%d", m_nextWaypoint.x, m_nextWaypoint.y);
         updateNextWaypoint(location, newDirection);
         updateDirection(newDirection, true);
      }

      const long stepDistance = std::max(abs(location.x - m_nextWaypoint.x), abs(location.y - m_nextWaypoint.y));

      if (distanceCovered < stepDistance)
      {
         // The Actor will not be able to make it to the next waypoint in this frame
         // Move towards the waypoint as much as possible.
         if(location.x < m_nextWaypoint.x)
         {
            location.x += distanceCovered;
            if(location.x > m_nextWaypoint.x) location.x = m_nextWaypoint.x;
         }
         else if(location.x > m_nextWaypoint.x)
         {
            location.x -= distanceCovered;
            if(location.x < m_nextWaypoint.x) location.x = m_nextWaypoint.x;
         }

         if(location.y < m_nextWaypoint.y)
         {
            location.y += distanceCovered;
            if(location.y > m_nextWaypoint.y) location.y = m_nextWaypoint.y;
         }
         else if(location.y > m_nextWaypoint.y)
         {
            location.y -= distanceCovered;
            if(location.y < m_nextWaypoint.y) location.y = m_nextWaypoint.y;
         }

         // Movement for this frame is finished
         m_actor.setLocation(location);
         return false;
      }

      // The Actor can reach the next waypoint in this frame
      distanceCovered -= stepDistance;

      DEBUG("Reached waypoint %d,%d", m_nextWaypoint.x, m_nextWaypoint.y);
      m_entityGrid.endMovement(&m_actor, m_lastWaypoint, m_nextWaypoint);
      m_movementBegun = false;

      // Update the current waypoint and dequeue it from the path
      location = m_nextWaypoint;
      m_path.pop_front();
   }

   return false;
}

void Actor::MoveOrder::draw() const
{
   if(DRAW_PATH)
   {
      if(m_path.empty()) return;

      glDisable(GL_TEXTURE_2D);
      glColor3f(1.0f, 0.0f, 0.0f);
      glBegin(GL_LINE_STRIP);
      for(const auto& node : m_path)
      {
         glVertex3d(node.x + TileEngine::TILE_SIZE / 2, node.y + TileEngine::TILE_SIZE / 2, 0);
      }
      glEnd();

      glEnable(GL_TEXTURE_2D);
   }
}
