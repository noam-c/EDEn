/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <future>
#include <list>
#include <vector>

#include "Grid.h"
#include "Rectangle.h"
#include "RoyFloydWarshallMatrices.h"

class Actor;
class EntityGrid;
class Map;

namespace geometry
{
   struct Size;
   struct Point2D;
};

struct TileState;

/**
 * The Pathfinder class binds to a Map and stores the locations of entities.
 * In doing so, it applies pathfinding algorithms to dynamically compute best paths around entities on the map.
 * Pathfinder instances provide an interface to entities like the actor and PlayerCharacter to detect collisions and route around them.
 *
 * @author Noam Chitayat
 */
class Pathfinder final
{
   /** The square root of 2. */
   static const float ROOT_2;

   /** The task tracking the asynchronous calculation of the grid's RFW matrices. */
   CancelableTask<RoyFloydWarshallMatrices> m_royFloydWarshallCalculation;

   /** The size (in pixels) of each tile. */
   int m_movementTileSize;

   /** The grid to compute paths on. */
   const Grid<TileState>* m_collisionGrid = nullptr;

   /** The bounds (in tiles) of the grid. */
   const geometry::Rectangle* m_collisionGridBounds = nullptr;

   /**
    * Runs the Roy-Floyd-Warshall algorithm on the initialized entity grid
    * to initialize the distance matrix and the successor matrix.
    */
   void initRoyFloydWarshallMatrices();

   /**
    * @return true iff the Roy-Floyd-Warshall algorithm has completed execution for the map.
    */
   bool isRoyFloydWarshallCalculationReady() const;

   public:
      /** A set of waypoints to move through in order to go from one point to another. */
      typedef std::list<geometry::Point2D> Path;

      /**
       * Constructor.
       */
      Pathfinder();

      /**
       * Initializes the pathfinder for the given entity grid.
       *
       * @param grid The entity grid to perform pathfinding computations on.
       * @param tileSize The size (in pixels) of each tile.
       * @param gridBounds The bounds of the grid.
       */
      void initialize(const Grid<TileState>& grid, int tileSize, const geometry::Rectangle& gridBounds);

      /**
       * Finds an ideal path from the source coordinates to the destination.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The ideal best path from the source point to the destination point.
       */
      Path findBestPath(const EntityGrid& entityGrid, const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size) const;

      /**
       * Finds the shortest path from the source coordinates to the destination
       * around all obstacles and entities.
       *
       * @param entityGrid The entity grid container.
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       * @param size The size of the moving entity.
       *
       * @return The shortest unobstructed path from the source point to the destination point.
       */
      Path findReroutedPath(const EntityGrid& entityGrid, const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size) const;

   private:
      /**
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).

       * @return A straight path from origin to goal, regardless of anything being in the way.
       */
      Path getStraightPath(const geometry::Point2D& src, const geometry::Point2D& dst) const;

      /**
       * Uses the successor matrix computed on Pathfinder construction to determine the best path.
       * This path does not take into account moving entities like Actors or the player, and does not take dynamically added obstacles into account.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The best path computed by the Roy-Floyd-Warshall algorithm.
       */
      Path findRFWPath(const geometry::Point2D& src, const geometry::Point2D& dst, const RoyFloydWarshallMatrices& rfwMatrices) const;

      /**
       * Manhattan distance heuristic for A* search.
       *
       * @param src The coordinates of the source.
       * @param dst The coordinates of the destination.
       *
       * @return the Manhattan distance (horizontal + vertical steps between src and dst).
       */
      static unsigned int getManhattanDistance(const geometry::Point2D& src, const geometry::Point2D& dst);

      /**
       * @param point The point to evaluate for neighbours.
       * @param bounds The rectangular area containing the point.
       *
       * @return A set of points adjacent to the point within the given bounds.
       */
      static std::vector<geometry::Point2D> getAdjacentPoints(const geometry::Point2D& point, const geometry::Rectangle& bounds);

      /**
       * Uses the A* algorithm to dynamically find the best possible path. Uses the Roy-Floyd-Warshall distance matrix as a heuristic when determining the best path.
       * This path will route around any dynamically added obstacles or moving entities based on their locations when this function is called.
       *
       * @param entityGrid The entity grid container.
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       * @param size The size of the moving entity.
       *
       * @return The best path computed by the A* algorithm.
       */
      Path findAStarPath(const EntityGrid& entityGrid, const geometry::Point2D& src, const geometry::Point2D& dst, const geometry::Size& size) const;

      /**
       * A node used in A* search.
       */
      class AStarNode;

      /**
       * Evaluate the neighbours of the evaluated node for A* search expansion.
       * Alters costs in the open set if cheaper paths are found,
       * and adds undiscovered tiles to the open set.
       *
       * @param entityState The state of the entity trying to move to the nodes.
       * @param entitySize The entity size.
       * @param evaluatedNode The node that is currently being evaluated.
       * @param destinationTile The goal point.
       * @param entityGrid The entity grid container.
       * @param rfwMatrices The Roy-Floyd-Warshall matrices for the map.
       * @param openSet The open set used to accumulate undiscovered points.
       * @param discovered A mapping from tile numbers to whether or not they have been discovered.
       */
      void evaluateAdjacentNodes(const TileState& entityState, const geometry::Size& entitySize, const std::shared_ptr<AStarNode>& evaluatedNode, const geometry::Point2D& destinationTile, const EntityGrid& entityGrid, const RoyFloydWarshallMatrices *const rfwMatrices, std::vector<std::shared_ptr<AStarNode>>& openSet, Grid<int>& discovered) const;
};

#endif
