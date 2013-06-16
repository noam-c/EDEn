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

   /**
    * Calculates the offset required to show the focal point as close to the center
    * of the camera viewport as possible.
    *
    * @return The offset to translate the camera with to make the focal point visible.
    */
   shapes::Point2D calculateCameraFocalOffset() const;
   
   public:
      /**
       * Constructor.
       */
      Camera();

      /**
       * Used to check if a given point is within the bounds of the camera's viewport.
       *
       * @param point The point to check (in viewport coordinates).
       *
       * @returns true iff the given point is within the camera's viewport.
       */
      bool isPointWithinViewport(const shapes::Point2D& point) const;

      /**
       * Reverses the camera's transformation of the scene to determine
       * the point in the scene represented by the given point.
       *
       * @param point The point to translate (in viewport coordinates).
       *
       * @return The given point translated into scene coordinates. 
       */
      shapes::Point2D getPointWithinScene(const shapes::Point2D& point) const;
   
      /**
       * Sets the viewport and scene bounds for the camera.
       *
       * @param newViewportSize The size of the camera's viewport (the area that will be drawn within it).
       * @param newSceneSize The size of the scene that the camera is pointing at.
       */
      void setViewBounds(const shapes::Size& newViewportSize, const shapes::Size& newSceneSize);

      /**
       * Gets the current point that the camera is centered around.
       *
       * @return The point that the camera is centering on (or origin if the focal point is not set).
       */
      const shapes::Point2D& Camera::getFocalPoint() const;

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
