/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "Rectangle.h"
#include "Point2D.h"
#include "Size.h"

namespace geometry
{
   Rectangle::Rectangle() :
      Rectangle(0, 0, 0, 0)
   {}

   Rectangle::Rectangle(const Point2D& topLeft, const Size& size) :
      Rectangle(topLeft.y,
                topLeft.x,
                topLeft.y + size.height,
                topLeft.x + size.width)
   {}

   Rectangle::Rectangle(const Point2D& topLeft, const Point2D& bottomRight) :
      Rectangle(topLeft.y,
                topLeft.x,
                bottomRight.y,
                bottomRight.x)
   {}

   Rectangle::Rectangle(std::tuple<int, int, int, int> edges) :
      Rectangle(std::get<0>(edges),
                std::get<1>(edges),
                std::get<2>(edges),
                std::get<3>(edges))
   {}

   Rectangle::Rectangle(int top, int left, int bottom, int right) :
      top(top),
      left(left),
      bottom(bottom),
      right(right)
   {}

   Rectangle Rectangle::translate(int x, int y) const
   {
      return {
         top + y,
         left + x,
         bottom + y,
         right + x
      };
   }

   bool Rectangle::isValid() const
   {
      return getWidth() != 0 && getHeight() != 0;
   }

   unsigned int Rectangle::getArea() const
   {
      return getWidth() * getHeight();
   }

   bool Rectangle::intersects(const Rectangle& other) const
   {
      return isValid()
         && other.isValid()
         && !(other.left > right
                || other.right < left
                || other.top > bottom
                || other.bottom < top);
   }

   Rectangle Rectangle::getIntersection(const Rectangle& other) const
   {
      if(!intersects(other) || !isValid() || !other.isValid())
      {
         return Rectangle();
      }

      return {
         std::max(top, other.top),
         std::max(left, other.left),
         std::min(bottom, other.bottom),
         std::min(right, other.right)
      };
   }

   bool Rectangle::contains(const Rectangle& other) const
   {
      return !(other.left < left
               || other.right >= right
               || other.top < top
               || other.bottom >= bottom);
   }

   bool Rectangle::contains(const Point2D& point) const
   {
      return !(point.x < left
               || point.x >= right
               || point.y < top
               || point.y >= bottom);
   }

   Size Rectangle::getSize() const
   {
      return Size(getWidth(), getHeight());
   }

   unsigned int Rectangle::getWidth() const
   {
      return right - left;
   }

   unsigned int Rectangle::getHeight() const
   {
      return bottom - top;
   }

   geometry::Point2D Rectangle::getTopLeftCorner() const
   {
      return { left, top };
   }

   geometry::Point2D Rectangle::getBottomRightCorner() const
   {
      return { right, bottom };
   }
};
