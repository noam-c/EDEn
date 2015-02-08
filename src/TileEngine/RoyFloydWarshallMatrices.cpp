/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2014 Noam Chitayat. All rights reserved.
 */

#include "RoyFloydWarshallMatrices.h"
#include "Point2D.h"
#include "TileState.h"
#include <limits>
#include <cstdlib>

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_PATHFINDER

const float RoyFloydWarshallMatrices::ROOT_2 = 1.41421356f;

shapes::Point2D RoyFloydWarshallMatrices::tileNumToCoords(int tileNum, int width)
{
   div_t result = div(tileNum, width);
   return shapes::Point2D(result.rem, result.quot);
}

int RoyFloydWarshallMatrices::coordsToTileNum(const shapes::Point2D& tileLocation, int width)
{
   return (tileLocation.y * width + tileLocation.x);
}

std::tuple<bool, shapes::Point2D> RoyFloydWarshallMatrices::getSuccessor(shapes::Point2D src, shapes::Point2D dst) const
{
   int srcTileNum = RoyFloydWarshallMatrices::coordsToTileNum(src, m_width);
   int dstTileNum = RoyFloydWarshallMatrices::coordsToTileNum(dst, m_width);

   int successorTileNum = m_successorMatrix(srcTileNum, dstTileNum);

   if(successorTileNum >= 0)
   {
      return std::make_tuple(true, tileNumToCoords(successorTileNum, m_width));
   }

   return std::make_tuple(false, shapes::Point2D::ORIGIN);
}

float RoyFloydWarshallMatrices::getDistance(shapes::Point2D src, shapes::Point2D dst) const
{
   int srcTileNum = RoyFloydWarshallMatrices::coordsToTileNum(src, m_width);
   int dstTileNum = RoyFloydWarshallMatrices::coordsToTileNum(dst, m_width);
   return m_distanceMatrix(srcTileNum, dstTileNum);
}

RoyFloydWarshallMatrices RoyFloydWarshallMatrices::calculateRoyFloydWarshallMatrices(const Grid<TileState>* grid, const shapes::Rectangle* gridBounds)
{
   RoyFloydWarshallMatrices matrices;

   if(!grid || !gridBounds)
   {
      T_T("Call made to calculateRoyFloydWarshallMatrices with null grid or null bounds.");
   }

   auto& distanceMatrix = matrices.m_distanceMatrix;
   auto& successorMatrix = matrices.m_successorMatrix;

   matrices.m_width = gridBounds->getWidth();
   const unsigned int NUM_TILES = gridBounds->getArea();
   const auto matrixSize = shapes::Size(NUM_TILES, NUM_TILES);

   distanceMatrix.resize(matrixSize);
   successorMatrix.resize(matrixSize);

   shapes::Point2D aTile;
   shapes::Point2D bTile;

   for(unsigned int a = 0; a < NUM_TILES; ++a)
   {
      aTile = tileNumToCoords(a, matrices.m_width);
      for(unsigned int b = 0; b < NUM_TILES; ++b)
      {
         if(a == b)
         {
            distanceMatrix(a, b) = 0;
            successorMatrix(a, b) = -1;
         }
         else
         {
            bTile = tileNumToCoords(b, matrices.m_width);

            bool xAdjacent = aTile.x - 1 <= bTile.x && bTile.x <= aTile.x + 1;
            bool yAdjacent = aTile.y - 1 <= bTile.y && bTile.y <= aTile.y + 1;

            bool adjacent = xAdjacent && yAdjacent;
            bool diagonallyAdjacent = aTile.x != bTile.x && aTile.y != bTile.y;
            bool aTileIsObstacle = (*grid)(aTile.x, aTile.y).entityType == TileState::EntityType::OBSTACLE;
            bool bTileIsObstacle = (*grid)(bTile.x, bTile.y).entityType == TileState::EntityType::OBSTACLE;

            distanceMatrix(a, b) = std::numeric_limits<float>::infinity();
            successorMatrix(a, b) = -1;

            if(adjacent && !aTileIsObstacle && !bTileIsObstacle)
            {
               if(diagonallyAdjacent)
               {
                  bool diagonalTraversalBlocked =
                  (*grid)(aTile.x, bTile.y).entityType == TileState::EntityType::OBSTACLE ||
                  (*grid)(bTile.x, aTile.y).entityType == TileState::EntityType::OBSTACLE;

                  if(!diagonalTraversalBlocked)
                  {
                     successorMatrix(a, b) = b;
                     distanceMatrix(a, b) = ROOT_2;
                  }
               }
               else
               {
                  successorMatrix(a, b) = b;
                  distanceMatrix(a, b) = 1;
               }
            }
         }
      }
   }

   for(unsigned int i = 0; i < NUM_TILES; ++i)
   {
      for(unsigned int a = 0; a < NUM_TILES; ++a)
      {
         for(unsigned int b = 0; b < NUM_TILES; ++b)
         {
            float distance = distanceMatrix(a, i) + distanceMatrix(i, b);
            if(distance < distanceMatrix(a, b))
            {
               distanceMatrix(a, b) = distance;
               successorMatrix(a, b) = successorMatrix(a, i);
            }
         }
      }
   }

   return matrices;
}
