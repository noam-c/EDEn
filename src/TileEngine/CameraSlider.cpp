/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "CameraSlider.h"
#include "Camera.h"
#include "DebugUtils.h"
#include <math.h>

const int debugFlag = DEBUG_TILE_ENG;

CameraSlider::CameraSlider(Camera& camera, const shapes::Point2D& origin, const shapes::Point2D& destination, double speed) :
   camera(camera),
   origin(camera.getClampedPoint(origin)),
   destination(camera.getClampedPoint(destination)),
   totalTimePassed(0)
{
   const double xMagnitude = this->destination.x - this->origin.x;
   const double yMagnitude = this->destination.y - this->origin.y;

   if(xMagnitude != 0 && yMagnitude != 0)
   {
      const double angle = atan2(yMagnitude, xMagnitude);
      xSpeed = cos(angle) * speed;
      ySpeed = sin(angle) * speed;
   }
   else if(xMagnitude == 0)
   {
      xSpeed = 0;
      ySpeed = speed;
   }
   else //if(yMagnitude == 0)
   {
      xSpeed = speed;
      ySpeed = 0;
   }
}

bool CameraSlider::resume(long timePassed)
{
   totalTimePassed += timePassed;

   const int xDistanceMoved = xSpeed * totalTimePassed;
   const int yDistanceMoved = ySpeed * totalTimePassed;

   shapes::Point2D focalPoint;

   focalPoint.x = destination.x < origin.x ?
      std::max(origin.x - xDistanceMoved, destination.x) :
      std::min(origin.x + xDistanceMoved, destination.x);

   focalPoint.y = destination.y < origin.y ?
      std::max(origin.y - yDistanceMoved, destination.y) :
      std::min(origin.y + yDistanceMoved, destination.y);

   camera.setFocalPoint(focalPoint);

   return focalPoint == destination;
}
