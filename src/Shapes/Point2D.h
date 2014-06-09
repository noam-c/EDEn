/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef POINT_2D_H
#define POINT_2D_H

#include <vector>

namespace shapes
{
   struct Rectangle;
   
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
      Point2D();

      /**
       * Constructor.
       * Creates a point set at (x,y).
       *
       * @param x The x-coordinate for the point.
       * @param y The y-coordinate for the point.
       */
      Point2D(int x, int y);
   
      /**
       * Equality operator.
       */
      bool operator==(const Point2D& rhs) const;
   
      /**
       * Inequality operator.
       */
      bool operator!=(const Point2D& rhs) const;
   
      /**
       * Multiplication operator.
       */
      Point2D operator*(int coeff) const;

      /**
       * Division operator.
       */
      Point2D operator/(int coeff) const;

      /** Origin point (0,0) */
      static const Point2D ORIGIN;
   };
};

#endif
