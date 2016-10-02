/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef GRID_ACTOR_MOVE_ORDER_H
#define GRID_ACTOR_MOVE_ORDER_H

#include "GridActorOrder.h"
#include "EntityGrid.h"

/**
 * An order that causes the GridActor to move to a specified
 * destination point on the map.
 *
 * @author Noam Chitayat
 */
class GridActor::MoveOrder final : public GridActor::Order
{
   /**
    * Tracks if the move order has calculated a path from
    * the Actor's current location to the destination.
    */
   bool m_pathInitialized = false;

   /**
    * Tracks if the Actor has begun movement
    * towards the next node in its path.
    */
   bool m_movementBegun = false;

   /** The destination that the Actor will move to. */
   const geometry::Point2D m_dst;

   /** The last node that the Actor successfully moved to. */
   geometry::Point2D m_lastWaypoint;

   /** The next node that the Actor will move to. */
   geometry::Point2D m_nextWaypoint;

   std::shared_ptr<Task> m_task;

   /** The grid that the Actor is moving along. */
   EntityGrid& m_entityGrid;

   /** The path that the Actor will use to get to the destination. */
   EntityGrid::Path m_path;

   /** Total distance for the character to move. */
   float m_cumulativeDistanceCovered = 0;

   /**
    * Update the direction that the Actor is facing, as well as the sprite used.
    *
    * @param newDirection The new direction to face.
    * @param moving True iff the Actor is moving toward another destination.
    */
   void updateDirection(geometry::Direction newDirection, bool moving);
   void updateNextWaypoint(geometry::Point2D location, geometry::Direction& direction);

   public:
      /**
       * Constructor.
       *
       * @param gridActor The Actor that will be following this Move order.
       * @param destination The point that the Actor will move to.
       * @param entityGrid The grid that the Actor is moving on.
       */
      MoveOrder(GridActor& gridActor, const std::shared_ptr<Task>& task, const geometry::Point2D& destination, EntityGrid& entityGrid);

      /**
       * Destructor.
       */
      ~MoveOrder() override;

      bool perform(long timePassed) override;
      void draw() const override;
};

#endif
