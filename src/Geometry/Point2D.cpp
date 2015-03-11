/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "Point2D.h"
#include "Rectangle.h"

namespace geometry
{
   const Point2D Point2D::ORIGIN(0, 0);

   bool Point2D::operator==(const Point2D& rhs) const noexcept
   {
      return x == rhs.x && y == rhs.y;
   }

   bool Point2D::operator!=(const Point2D& rhs) const noexcept
   {
      return x != rhs.x || y != rhs.y;
   }

   Point2D Point2D::operator*(int coeff) const
   {
      return { x * coeff, y * coeff };
   }

   Point2D Point2D::operator/(int coeff) const
   {
      return { x / coeff, y / coeff };
   }
};