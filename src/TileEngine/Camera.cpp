#include "Camera.h"
#include "GraphicsUtil.h"
#include "Actor.h"
#include "Point2D.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TILE_ENG;

Camera::Camera() :
   cameraApplied(false),
   focalPoint(0, 0),
   focalPointSet(false),
   offset(0, 0)
{
}

void Camera::setFocalPoint(const shapes::Point2D& point)
{
   focalPoint = point;
   focalPointSet = true;
}

void Camera::clearFocalPoint()
{
   focalPoint.x = 0;
   focalPoint.y = 0;

   focalPointSet = false;
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

      if (focalPointSet)
      {
         shapes::Point2D cameraFocalPoint;
         
         if(offset.x == 0)
         {
            // Offset the camera's focal point by half the screen width to
            // center the focal point horizontally
            cameraFocalPoint.x = focalPoint.x - (viewportSize.width / 2);
            cameraFocalPoint.x = std::max(cameraFocalPoint.x, 0);

            const signed int rightCameraBoundary = static_cast<signed int>(sceneSize.width - viewportSize.width);
            cameraFocalPoint.x = std::min(cameraFocalPoint.x, rightCameraBoundary);
         }

         if(offset.y == 0)
         {
            // Offset the camera's focal point by half the screen height to
            // center the focal point vertically
            cameraFocalPoint.y = focalPoint.y - (viewportSize.height / 2);
            cameraFocalPoint.y = std::max(cameraFocalPoint.y, 0);

            const signed int bottomCameraBoundary = static_cast<signed int>(sceneSize.height - viewportSize.height);
            cameraFocalPoint.y = std::min(cameraFocalPoint.y, bottomCameraBoundary);
         }

         // Perform an inverse translation from the focal point to shift
         // the scene to the camera
         glTranslated(-cameraFocalPoint.x, -cameraFocalPoint.y, 0.0);
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