/**
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2011 Noam Chitayat. All rights reserved.
 */

#ifndef POINT_2D_H
#define POINT_2D_H

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
   Point2D() : x(0), y(0) {}
   
   /**
    * Constructor.
    * Creates a point set at (x,y).
    *
    * @param x The x-coordinate for the point.
    * @param y The y-coordinate for the point.
    */
   Point2D(int x, int y) : x(x), y(y) {}
};

#endif
