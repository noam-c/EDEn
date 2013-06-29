#include "Camera.h"
#include "GraphicsUtil.h"
#include "Actor.h"
#include "Point2D.h"
#include "Rectangle.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TILE_ENG;

Camera::Camera() :
   cameraApplied(false),
   focalPoint(0, 0),
   focalPointSet(false),
   offset(0, 0)
{
}

shapes::Point2D Camera::getFocalPoint() const
{
   return focalPointSet ? focalPoint : shapes::Point2D::ORIGIN;
}

void Camera::setFocalPoint(const shapes::Point2D& point)
{
   focalPoint = point;
   focalPointSet = true;
}

void Camera::clearFocalPoint()
{
   focalPoint = shapes::Point2D::ORIGIN;
   focalPointSet = false;
}

bool Camera::isPointWithinViewport(const shapes::Point2D& point) const
{
   return shapes::Rectangle(offset, viewportSize).contains(point);
}

shapes::Point2D Camera::getPointWithinScene(const shapes::Point2D& point) const
{
   const shapes::Point2D cameraFocalOffset = calculateCameraFocalOffset();
   return shapes::Point2D(
      point.x - offset.x - cameraFocalOffset.x,
      point.y - offset.y - cameraFocalOffset.y);
}

shapes::Point2D Camera::getClampedPoint(const shapes::Point2D& point) const
{
   return shapes::Point2D(
      std::min(std::max(point.x, static_cast<signed int>(viewportSize.width / 2)), static_cast<signed int>(sceneSize.width - viewportSize.width / 2)),
      std::min(std::max(point.y, static_cast<signed int>(viewportSize.height / 2)), static_cast<signed int>(sceneSize.height - viewportSize.height / 2)));
}

void Camera::setViewBounds(const shapes::Size& newViewportSize, const shapes::Size& newSceneSize)
{
   viewportSize = newViewportSize;
   sceneSize = newSceneSize;
   offset.x = sceneSize.width <  viewportSize.width ?
              (viewportSize.width - sceneSize.width) / 2 : 0;

   offset.y = sceneSize.height < viewportSize.height ?
              (viewportSize.height - sceneSize.height) / 2 : 0;
}

shapes::Point2D Camera::calculateCameraFocalOffset() const
{
   shapes::Point2D cameraFocalOffset = shapes::Point2D::ORIGIN;
   if (focalPointSet)
   {
      if(offset.x == 0)
      {
         // Offset the camera's focal point by half the screen width to
         // center the focal point horizontally
         cameraFocalOffset.x = focalPoint.x - (viewportSize.width / 2);
         cameraFocalOffset.x = std::max(cameraFocalOffset.x, 0);
         
         const signed int rightCameraBoundary = static_cast<signed int>(sceneSize.width - viewportSize.width);
         cameraFocalOffset.x = -std::min(cameraFocalOffset.x, rightCameraBoundary);
      }
      
      if(offset.y == 0)
      {
         // Offset the camera's focal point by half the screen height to
         // center the focal point vertically
         cameraFocalOffset.y = focalPoint.y - (viewportSize.height / 2);
         cameraFocalOffset.y = std::max(cameraFocalOffset.y, 0);
         
         const signed int bottomCameraBoundary = static_cast<signed int>(sceneSize.height - viewportSize.height);
         cameraFocalOffset.y = -std::min(cameraFocalOffset.y, bottomCameraBoundary);
      }
   }
   
   return cameraFocalOffset;
}

void Camera::apply()
{
   if(!cameraApplied)
   {
      // Apply an absolute offset (to center all the drawn elements)
      GraphicsUtil::getInstance()->setAbsoluteOffset(offset.x, offset.y);
      glPushMatrix();

      // Create a scissor region around the camera's view bounds
      // To prevent any elements from being drawn outside the camera's specified view.
      glPushAttrib(GL_SCISSOR_BIT);
      glEnable(GL_SCISSOR_TEST);

      const int scissorYOffset = GraphicsUtil::getInstance()->getHeight() - viewportSize.height;
      glScissor(0, scissorYOffset, viewportSize.width, viewportSize.height);

      shapes::Point2D cameraFocalOffset = calculateCameraFocalOffset();

      if(cameraFocalOffset != shapes::Point2D::ORIGIN)
      {
         // Perform an inverse translation from the focal point to shift
         // the scene to the camera
         glTranslated(cameraFocalOffset.x, cameraFocalOffset.y, 0.0);
      }

      cameraApplied = true;
   }
}

void Camera::reset()
{
   if(cameraApplied)
   {
      // Reset the scissor attribute
      glDisable(GL_SCISSOR_TEST);
      glPopAttrib();

      // Reset the camera translation
      glPopMatrix();

      GraphicsUtil::getInstance()->resetAbsoluteOffset();
      cameraApplied = false;
   }
}

Camera::~Camera()
{
}