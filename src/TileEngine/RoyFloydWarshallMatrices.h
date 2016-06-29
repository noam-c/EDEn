/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef RFW_MATRICES_H
#define RFW_MATRICES_H

#include "CancelableTask.h"
#include "Grid.h"
#include "Rectangle.h"

namespace geometry
{
   struct Point2D;
};

struct TileState;

/**
 * Holds the results of running the Roy-Floyd-Warshall
 * algorithm on a grid.
 *
 * @author Noam Chitayat
 */
class RoyFloydWarshallMatrices
{
   /** The square root of 2. */
   static const float ROOT_2;

   /** The width of the grid. */
   int m_width;

   /** The Roy-Floyd-Warshall distance matrix. This 2D array holds best-path distances between all tiles. */
   Grid<float> m_distanceMatrix;

   /** The Roy-Floyd-Warshall successor matrix. This 2D array holds the best tile to move to, given a source and a destination. */
   Grid<int> m_successorMatrix;

   /**
    * @param tileNum The matrix index of the tile.
    * @param width The width of the grid.
    *
    * @return The 2D point on the grid corresponding to the given tile number.
    */
   static geometry::Point2D tileNumToCoords(int tileNum, int width);

   /**
    * @param tileLocation A 2D point on the grid.
    * @param width The width of the grid.
    *
    * @return The matrix index of the given tile.
    */
   static int coordsToTileNum(const geometry::Point2D& tileLocation, int width);

   /**
    * Constructor is private.
    * Please use <code>RoyFloydWarshallMatrices::calculateRoyFloydWarshallMatrices</code>
    * to create an instance.
    */
   RoyFloydWarshallMatrices() = default;

   public:
      /**
       * Gets the next tile to traverse on the path between
       * the given source and destination tiles.
       *
       * @param src The origin tile from which the actor will depart.
       * @param dst The destination tile.
       *
       * @return a tuple object containing a success boolean (1) and, if successful, a tile to move to (2).
       */
      std::tuple<bool, geometry::Point2D> getSuccessor(geometry::Point2D src, geometry::Point2D dst) const;

      /**
       * Gets the shortest distance between
       * the given source and destination tiles.
       *
       * @param src The origin tile from which the actor will depart.
       * @param dst The destination tile.
       *
       * @return The distance between the two points given.
       */
      float getDistance(geometry::Point2D src, geometry::Point2D dst) const;

      /**
       * @param grid A grid of free spaces and obstacles.
       * @param gridBounds The rectangle representing the bounds of the grid.
       * @param cancelCalculation An atomic flag used to determine if the calculation was canceled in flight.
       *
       * @return the results of the RFW algorithm for the given grid.
       */
      static RoyFloydWarshallMatrices calculateRoyFloydWarshallMatrices(const Grid<TileState>* grid, const geometry::Rectangle* gridBounds, std::atomic<bool>& cancelCalculation);
};

#endif
