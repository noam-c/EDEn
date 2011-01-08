#ifndef __POINT_2D_H_
#define __POINT_2D_H_

struct Point2D
{
   int x;
   int y;

   Point2D() : x(0), y(0) {}
   Point2D(int x, int y) : x(x), y(y) {}
};

#endif