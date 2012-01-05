/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

struct Point2D;

struct Rectangle
{
   int top;
   int left;
   int bottom;
   int right;

   Rectangle(Point2D topLeft, int width, int height);
   Rectangle(Point2D topLeft, Point2D bottomRight);
   Rectangle(int top, int left, int bottom, int right);

   bool intersects(const Rectangle& other) const;
};

#endif
