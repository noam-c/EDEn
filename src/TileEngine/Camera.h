/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "Point2D.h"
#include "Size.h"

/**
 * Manages the camera for a scene, exposing a means for a GameState
 * to offset the scene and to center around a specified focal point
 * during drawing.
 *
 * @author Noam Chitayat
 */
class Camera
{
   /** True iff the camera's transformations have been applied to the renderer. */
   bool cameraApplied;

   /** The focal point for the camera to center around. */
   shapes::Point2D focalPoint;

   /** True iff there is a focal point set for the camera to center around. */
   bool focalPointSet;

   /** The offset from the top-left corner of the window to the top-left corner of the camera's viewport. */
   shapes::Point2D offset;

   /** The size of the camera's viewport. */
   shapes::Size viewportSize;

   /** The size of the scene that the camera is pointing at. */
   shapes::Size sceneSize;

   public:
      /**
       * Constructor.
       */
      Camera();

      /**
       * Sets the viewport and scene bounds for the camera.
       *
       * @param newViewportSize The size of the camera's viewport (the area that will be drawn within it).
       * @param newSceneSize The size of the scene that the camera is pointing at.
       */
      void setViewBounds(const shapes::Size& newViewportSize, const shapes::Size& newSceneSize);

      /**
       * Sets a new point for the camera to center around.
       *
       * @param point The point to center on if the viewport is smaller than the scene on either dimension.
       */
      void setFocalPoint(const shapes::Point2D& point);

      /**
       * Clears the set focal point, resetting the camera so that it doesn't center on anything.
       */
      void clearFocalPoint();

      /**
       * Applies the camera's transformation to the scene.
       *
       * NOTE: This method makes changes to the renderer and must therefore be
       *       followed with a call to Camera::reset after the camera's contents
       *       are drawn.
       * NOTE: If the camera's transformations are already applied
       *       to the scene, then this is a no-op.
       */
      void apply();

      /**
       * Undoes the camera's transformation on the scene.
       *
       * NOTE: This method only resets changes to the renderer when it proceeds a
       *       call to Camera::apply.
       * NOTE: If the camera's transformations are not applied
       *       to the scene, then this is a no-op.
       */
      void reset();

      /**
       * Destructor.
       */
      ~Camera();
};

#endif