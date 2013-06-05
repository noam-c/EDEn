/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef CAMERA_SLIDER_H
#define CAMERA_SLIDER_H

#include "Coroutine.h"
#include "Point2D.h"

class Camera;

/**
 * A CameraSlider is a Coroutine that moves the camera to a specified location over a period of time.
 *
 * @author Noam Chitayat
 */
class CameraSlider : public Coroutine
{
   /** The camera to manipulate. */
   Camera& camera;

   const shapes::Point2D origin;
   const shapes::Point2D destination;

   /** The amount of time that has passed since the slide began. */
   long totalTimePassed;
   
   /** The speed of the camera slide on the x-axis. */
   double xSpeed;

   /** The speed of the camera slide on the y-axis. */
   double ySpeed;

   public:
      /**
       * Constructor.
       */
      CameraSlider(Camera& camera, const shapes::Point2D& origin, const shapes::Point2D& destination, double speed);

      /**
       * Resume this camera slide.
       *
       * @param timePassed The time that has passed since the last frame.
       *
       * @return true iff the slide has completed.
       */
      bool resume(long timePassed);
};

#endif
