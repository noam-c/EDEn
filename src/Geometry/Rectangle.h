/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <tuple>

namespace geometry
{
   struct Point2D;
   struct Size;

   /**
    * A rectangle, represented by its top, left, bottom and right edges.
    *
    * @author Noam Chitayat
    */
   struct Rectangle final
   {
      /** The Y-location of the top edge of the rectangle. */
      int top = 0;

      /** The X-location of the left edge of the rectangle. */
      int left = 0;

      /** The Y-location of the bottom edge of the rectangle. */
      int bottom = 0;

      /** The X-location of the right edge of the rectangle. */
      int right = 0;

      /**
       * Default constructor.
       * Creates a rectangle at origin with no size.
       */
      Rectangle();

      /**
       * Constructor.
       *
       * @param topLeft The top-left corner of the rectangle.
       * @param size The dimensions of the rectangle.
       */
      Rectangle(const Point2D& topLeft, const Size& size);

      /**
       * Constructor.
       *
       * @param topLeft The top-left corner of the rectangle.
       * @param bottomRight The bottom-right corner of the rectangle.
       */
      Rectangle(const Point2D& topLeft, const Point2D& bottomRight);
      
      /**
       * Constructor.
       *
       * @param edges The top, left, bottom, and right edges.
       */
      Rectangle(std::tuple<int, int, int, int> edges);

      /**
       * Constructor.
       *
       * @param top The Y-location of the top edge of the rectangle.
       * @param left The X-location of the left edge of the rectangle.
       * @param bottom The Y-location of the bottom edge of the rectangle.
       * @param right The X-location of the right edge of the rectangle.
       */
      Rectangle(int top, int left, int bottom, int right);

      /**
       * Get a version of this Rectangle with a translation applied.
       *
       * @param x The amount to translate to the right.
       * @param y The amount to translate downward.
       *
       * @return A clone of this Rectangle, with its edges translated by x and y.
       */
      Rectangle translate(int x, int y) const;

      /**
       * @return true iff this Rectangle is valid (it has height and width).
       */
      bool isValid() const;

      /**
       * @return The area of the rectangle (width * height).
       */
      unsigned int getArea() const;

      /**
       * Test for intersection with another Rectangle.
       *
       * @param other The Rectangle to check for intersection with this Rectangle.
       * @return true iff this Rectangle intersects with the other Rectangle.
       */
      bool intersects(const Rectangle& other) const;

      /**
       * Gets the overlapping section between two Rectangles.
       *
       * @param other The Rectangle to check for intersection with this Rectangle.
       * @return a Rectangle representing the overlap between the two Rectangles.
       */
      Rectangle getIntersection(const Rectangle& other) const;

      /**
       * Test for containment of another Rectangle.
       *
       * @param other The Rectangle to check for containment within this Rectangle.
       * @return true iff this Rectangle contains the other Rectangle.
       */
      bool contains(const Rectangle& other) const;

      /**
       * Test for containment of a point in 2D space.
       *
       * @param point The point to check for containment within this Rectangle.
       * @return true iff this Rectangle contains the point.
       */
      bool contains(const Point2D& point) const;

      /**
       * @return the dimensions of the rectangle.
       */
      Size getSize() const;

      /**
       * @return the width of the rectangle.
       */
      unsigned int getWidth() const;

      /**
       * @return the height of the rectangle.
       */
      unsigned int getHeight() const;

      /**
       * @return the location of the top left corner of the rectangle.
       */
      geometry::Point2D getTopLeftCorner() const;

      /**
       * @return the location of the bottom right corner of the rectangle.
       */
      geometry::Point2D getBottomRightCorner() const;
   };
};

#endif
