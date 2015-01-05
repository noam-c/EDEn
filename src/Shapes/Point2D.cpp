/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
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
};