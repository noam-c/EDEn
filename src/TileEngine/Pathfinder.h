/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <list>
#include <vector>

#include "Rectangle.h"

class Actor;
class EntityGrid;
class Map;

namespace shapes
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
class Pathfinder
{
   /** The square root of 2. */
   static const float ROOT_2;

   /** Floating-point notation for infinity. */
   static const float INFINITY;

   /** The Roy-Floyd-Warshall distance matrix. This 2D array holds best-path distances between all tiles. */
   float** m_distanceMatrix;

   /** The Roy-Floyd-Warshall successor matrix. This 2D array holds the best tile to move to, given a source and a destination. */
   int** m_successorMatrix;
   
   /** The size (in pixels) of each tile. */
   int m_movementTileSize;
   
   /** The grid to compute paths on. */
   TileState** m_collisionGrid;
   
   /** The bounds (in tiles) of the grid. */
   shapes::Rectangle m_collisionGridBounds;
   
   /**
    * Convert a tile number into pixel coordinates.
    *
    * @param tileNum The tile number when counting the tiles from left to right, then top to bottom.
    */
   inline shapes::Point2D tileNumToPixels(int tileNum) const;
   
   /**
    * Convert pixel coordinates into a tile number.
    *
    * @param pixelLocation The coordinates of the location (in pixels)
    */
   inline int pixelsToTileNum(const shapes::Point2D& pixelLocation) const;
   
   /**
    * Convert a tile number into tile coordinates.
    *
    * @param tileNum The tile number when counting the tiles from left to right, then top to bottom.
    */
   inline shapes::Point2D tileNumToCoords(int tileNum) const;
   
   /**
    * Convert tile coordinates into a tile number.
    *
    * @param tileLocation The coordinates of the location (in tiles)
    */
   inline int coordsToTileNum(const shapes::Point2D& tileLocation) const;
   
   /** 
    * Runs the Roy-Floyd-Warshall algorithm on the initialized entity grid
    * to initialize the distance matrix and the successor matrix.
    */
   void initRoyFloydWarshallMatrices();

   /**
    * Clean up the distance and successor matrices.
    */
   void deleteRoyFloydWarshallMatrices();
   
   public:
      /** A set of waypoints to move through in order to go from one point to another. */
      typedef std::list<shapes::Point2D> Path;

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
      void initialize(TileState** grid, int tileSize, const shapes::Rectangle& gridBounds);

      /**
       * Destructor.
       */
      ~Pathfinder();

      /**
       * Finds an ideal path from the source coordinates to the destination.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The ideal best path from the source point to the destination point.
       */
      Path findBestPath(const shapes::Point2D& src, const shapes::Point2D& dst) const;
      
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
      Path findReroutedPath(const EntityGrid& entityGrid, const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size) const;

   private:
      /**
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).

       * @return A straight path from origin to goal, regardless of anything being in the way.
       */
      Path getStraightPath(const shapes::Point2D& src, const shapes::Point2D& dst) const;

      /**
       * Uses the successor matrix computed on Pathfinder construction to determine the best path.
       * This path does not take into account moving entities like Actors or the player, and does not take dynamically added obstacles into account.
       *
       * @param src The coordinates of the source (in pixels).
       * @param dst The coordinates of the destination (in pixels).
       *
       * @return The best path computed by the Roy-Floyd-Warshall algorithm. 
       */
      Path findRFWPath(const shapes::Point2D& src, const shapes::Point2D& dst) const;

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
      Path findAStarPath(const EntityGrid& entityGrid, const shapes::Point2D& src, const shapes::Point2D& dst, const shapes::Size& size) const;

      /**
       * A node used in A* search.
       */
      class AStarPoint;
      
      /**
       * Evaluate a group of neighbour nodes in A* search.
       * Alters costs in the open set if cheaper paths are found,
       * and adds undiscovered tiles to the open set.
       *
       * @param entityGrid The entity grid container.
       * @param entityState The state of the entity trying to move to the nodes.
       * @param adjacentNodes The neighbour nodes to evaluate.
       * @param evaluatedPoint The point that is currently being evaluated.
       * @param traversalCost The cost to move from evaluatedPoint to any of the adjacent nodes.
       * @param destinationTileNum The tile number of the goal point.
       * @param openSet The open set used to accumulate undiscovered points.
       * @param discovered A mapping from tile numbers to whether or not they have been discovered.
       * @param size The entity Size.
       * @param diagonalNodes Whether or not the nodes being evaluated are diagonal from the evaluated point.
       */
      void evaluateAdjacentNodes(const EntityGrid& entityGrid, const TileState& entityState, const std::vector<shapes::Point2D>& adjacentNodes, const AStarPoint* evaluatedPoint, float traversalCost, int destinationTileNum, std::vector<AStarPoint*>& openSet, std::vector<bool>& discovered, const shapes::Size& size, bool diagonalNodes = false) const;
};

#endif
