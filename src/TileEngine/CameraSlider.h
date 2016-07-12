/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef CAMERA_SLIDER_H
#define CAMERA_SLIDER_H

#include <memory>

#include "Coroutine.h"
#include "Point2D.h"

class Camera;
class Task;

/**
 * A CameraSlider is a Coroutine that moves the camera to a specified location over a period of time.
 *
 * @author Noam Chitayat
 */
class CameraSlider : public Coroutine
{
   /** The camera to manipulate. */
   Camera& m_camera;

   const geometry::Point2D m_origin;
   const geometry::Point2D m_destination;
   std::shared_ptr<Task> m_task;

   /** The amount of time that has passed since the slide began. */
   long m_totalTimePassed;

   /** The speed of the camera slide on the x-axis. */
   double m_xSpeed;

   /** The speed of the camera slide on the y-axis. */
   double m_ySpeed;

   public:
      /**
       * Constructor.
       */
      CameraSlider(Camera& camera, const geometry::Point2D& origin, const geometry::Point2D& destination, double speed, const std::shared_ptr<Task>& task);

      /**
       * Resume this camera slide.
       *
       * @param timePassed The time that has passed since the last frame.
       *
       * @return true iff the slide has completed.
       */
      bool resume(long timePassed) override;
};

#endif
