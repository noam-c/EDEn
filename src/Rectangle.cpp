/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "Rectangle.h"
#include "Point2D.h"

Rectangle::Rectangle(Point2D topLeft, int width, int height) : top(topLeft.y), left(topLeft.x), bottom(top + height), right(left + width) {}
Rectangle::Rectangle(Point2D topLeft, Point2D bottomRight) : top(topLeft.y), left(topLeft.x), bottom(bottomRight.y), right(bottomRight.x) {}
Rectangle::Rectangle(int top, int left, int bottom, int right) : top(top), left(left), bottom(bottom), right(right) {}

bool Rectangle::intersects(const Rectangle& other) const
{
   return !(other.left > right
             || other.right < left
             || other.top > bottom
             || other.bottom < top
             );
}
