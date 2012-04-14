/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

namespace shapes
{
   struct Point2D;
   struct Size;

   struct Rectangle
   {
      int top;
      int left;
      int bottom;
      int right;
      
      Rectangle();
      Rectangle(const Point2D& topLeft, const Size& size);
      Rectangle(const Point2D& topLeft, const Point2D& bottomRight);

      /**
       * @return The area of the rectangle (width * height).
       */
      unsigned int getArea() const;
      
      bool intersects(const Rectangle& other) const;
      bool contains(const Rectangle& other) const;
      bool contains(const Point2D& other) const;

      Size getSize() const;
      unsigned int getWidth() const;
      unsigned int getHeight() const;
   };
};

#endif
