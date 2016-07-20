/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include <math.h>
#include <algorithm>

#include "CameraSlider.h"
#include "Camera.h"
#include "DebugUtils.h"
#include "Task.h"

#define DEBUG_FLAG DEBUG_TILE_ENG

CameraSlider::CameraSlider(Camera& camera, const geometry::Point2D& origin, const geometry::Point2D& destination, double speed, const std::shared_ptr<Task>& task) :
   m_camera(camera),
   m_origin(camera.getClampedPoint(origin)),
   m_destination(camera.getClampedPoint(destination)),
   m_task(task)
{
   const double xMagnitude = m_destination.x - m_origin.x;
   const double yMagnitude = m_destination.y - m_origin.y;

   if(xMagnitude != 0 && yMagnitude != 0)
   {
      const double angle = atan2(yMagnitude, xMagnitude);
      m_xSpeed = cos(angle) * speed;
      m_ySpeed = sin(angle) * speed;
   }
   else if(xMagnitude == 0)
   {
      m_xSpeed = 0;
      m_ySpeed = speed;
   }
   else //if(yMagnitude == 0)
   {
      m_xSpeed = speed;
      m_ySpeed = 0;
   }
}

bool CameraSlider::resume(long timePassed)
{
   m_totalTimePassed += timePassed;

   const int xDistanceMoved = m_xSpeed * m_totalTimePassed;
   const int yDistanceMoved = m_ySpeed * m_totalTimePassed;

   geometry::Point2D focalPoint;

   focalPoint.x = m_destination.x < m_origin.x ?
      std::max(m_origin.x - xDistanceMoved, m_destination.x) :
      std::min(m_origin.x + xDistanceMoved, m_destination.x);

   focalPoint.y = m_destination.y < m_origin.y ?
      std::max(m_origin.y - yDistanceMoved, m_destination.y) :
      std::min(m_origin.y + yDistanceMoved, m_destination.y);

   m_camera.setFocalPoint(focalPoint);

   if(focalPoint == m_destination)
   {
      if(m_task)
      {
         m_task->complete();
      }
      
      return true;
   }
   
   return false;
}
