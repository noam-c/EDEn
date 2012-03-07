/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Point2D.h"

namespace shapes
{
   Point2D::Point2D() : x(0), y(0) {}

   Point2D::Point2D(int x, int y) : x(x), y(y) {}

   bool Point2D::operator==(const Point2D& rhs) const
   {
      return x == rhs.x && y == rhs.y;
   }

   bool Point2D::operator!=(const Point2D& rhs) const
   {
      return x != rhs.x || y != rhs.y;
   }

   Point2D Point2D::operator*(int coeff) const
   {
      return Point2D(x * coeff, y * coeff);
   }

   Point2D Point2D::operator/(int coeff) const
   {
      return Point2D(x / coeff, y / coeff);
   }

   std::vector<Point2D> Point2D::getDiagonallyAdjacentPoints(const Point2D& point, int mapWidth, int mapHeight)
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

   std::vector<Point2D> Point2D::getLaterallyAdjacentPoints(const Point2D& point, int mapWidth, int mapHeight)
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