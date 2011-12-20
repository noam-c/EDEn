/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef POINT_2D_H
#define POINT_2D_H

#include <vector>

/**
 * A point in 2D space.
 *
 * @author Noam Chitayat
 */
struct Point2D
{
   /** The x-coordinate of this point. */
   int x;
   /** The y-coordinate of this point. */
   int y;

   /**
    * Constructor.
    * Creates a point set at origin (0,0).
    */
   Point2D() : x(0), y(0) {}
   
   /**
    * Constructor.
    * Creates a point set at (x,y).
    *
    * @param x The x-coordinate for the point.
    * @param y The y-coordinate for the point.
    */
   Point2D(int x, int y) : x(x), y(y) {}
   
   /**
    * Equality operator.
    */
   bool operator==(const Point2D& rhs) const
   {
      return x == rhs.x && y == rhs.y;
   }

   static std::vector<Point2D> getDiagonallyAdjacentPoints(const Point2D& point, int mapWidth, int mapHeight)
   {
      std::vector<Point2D> diagonalPoints;
      
      if(point.x > 0 && point.y > 0)
      {
         diagonalPoints.push_back(Point2D(point.x - 1, point.y - 1));
      }
      
      if(point.x > 0 && point.y < mapHeight - 1)
      {
         diagonalPoints.push_back(Point2D(point.x - 1, point.y + 1));
      }
      
      if(point.x < mapWidth - 1 && point.y > 0)
      {
         diagonalPoints.push_back(Point2D(point.x + 1, point.y - 1));
      }
      
      if(point.x < mapWidth - 1 && point.y < mapHeight - 1)
      {
         diagonalPoints.push_back(Point2D(point.x + 1, point.y + 1));
      }
      
      return diagonalPoints;
   }
   
   static std::vector<Point2D> getLaterallyAdjacentPoints(const Point2D& point, int mapWidth, int mapHeight)
   {
      std::vector<Point2D> lateralPoints;
      
      if(point.x > 0)
      {
         lateralPoints.push_back(Point2D(point.x - 1, point.y));
      }
      
      if(point.x < mapWidth - 1)
      {
         lateralPoints.push_back(Point2D(point.x + 1, point.y));
      }
      
      if(point.y > 0)
      {
         lateralPoints.push_back(Point2D(point.x, point.y - 1));
      }
      
      if(point.y < mapHeight - 1)
      {
         lateralPoints.push_back(Point2D(point.x, point.y + 1));
      }
      
      return lateralPoints;
   }
};

#endif
