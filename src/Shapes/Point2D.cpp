/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "Point2D.h"
#include "Rectangle.h"

namespace shapes
{
   const Point2D Point2D::ORIGIN(0, 0);

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

   std::vector<Point2D> Point2D::getDiagonallyAdjacentPoints(const Point2D& point, const Rectangle& bounds)
   {
      std::vector<Point2D> diagonalPoints;
   
      if(point.x > bounds.left && point.y > bounds.top)
      {
         diagonalPoints.emplace_back(point.x - 1, point.y - 1);
      }
   
      if(point.x > bounds.left && point.y < bounds.bottom - 1)
      {
         diagonalPoints.emplace_back(point.x - 1, point.y + 1);
      }
   
      if(point.x < bounds.right - 1 && point.y > bounds.top)
      {
         diagonalPoints.emplace_back(point.x + 1, point.y - 1);
      }
   
      if(point.x < bounds.right - 1 && point.y < bounds.bottom - 1)
      {
         diagonalPoints.emplace_back(point.x + 1, point.y + 1);
      }
   
      return diagonalPoints;
   }

   std::vector<Point2D> Point2D::getLaterallyAdjacentPoints(const Point2D& point, const Rectangle& bounds)
   {
      std::vector<Point2D> lateralPoints;
   
      if(point.x > bounds.left)
      {
         lateralPoints.emplace_back(point.x - 1, point.y);
      }
   
      if(point.x < bounds.right - 1)
      {
         lateralPoints.emplace_back(point.x + 1, point.y);
      }
   
      if(point.y > bounds.top)
      {
         lateralPoints.emplace_back(point.x, point.y - 1);
      }
   
      if(point.y < bounds.bottom - 1)
      {
         lateralPoints.emplace_back(point.x, point.y + 1);
      }
   
      return lateralPoints;
   }
};