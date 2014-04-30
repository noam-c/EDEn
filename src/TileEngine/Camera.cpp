#include "Camera.h"
#include "GraphicsUtil.h"
#include "Actor.h"
#include "Point2D.h"
#include "Rectangle.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TILE_ENG;

Camera::Camera() :
   m_cameraApplied(false),
   m_focalPoint(0, 0),
   m_focalPointSet(false),
   m_offset(0, 0)
{
}

shapes::Point2D Camera::getFocalPoint() const
{
   return m_focalPointSet ? m_focalPoint : shapes::Point2D::ORIGIN;
}

void Camera::setFocalPoint(const shapes::Point2D& point)
{
   m_focalPoint = point;
   m_focalPointSet = true;
}

void Camera::clearFocalPoint()
{
   m_focalPoint = shapes::Point2D::ORIGIN;
   m_focalPointSet = false;
}

bool Camera::isPointWithinViewport(const shapes::Point2D& point) const
{
   return shapes::Rectangle(m_offset, m_viewportSize).contains(point);
}

shapes::Point2D Camera::getPointWithinScene(const shapes::Point2D& point) const
{
   const shapes::Point2D cameraFocalOffset = calculateCameraFocalOffset();
   return shapes::Point2D(
      point.x - m_offset.x - cameraFocalOffset.x,
      point.y - m_offset.y - cameraFocalOffset.y);
}

shapes::Point2D Camera::getClampedPoint(const shapes::Point2D& point) const
{
   return shapes::Point2D(
      std::min(std::max(point.x, static_cast<signed int>(m_viewportSize.width / 2)), static_cast<signed int>(m_sceneSize.width - m_viewportSize.width / 2)),
      std::min(std::max(point.y, static_cast<signed int>(m_viewportSize.height / 2)), static_cast<signed int>(m_sceneSize.height - m_viewportSize.height / 2)));
}

void Camera::setViewBounds(const shapes::Size& viewportSize, const shapes::Size& sceneSize)
{
   m_viewportSize = viewportSize;
   m_sceneSize = sceneSize;
   m_offset.x = m_sceneSize.width <  m_viewportSize.width ?
              (m_viewportSize.width - m_sceneSize.width) / 2 : 0;

   m_offset.y = m_sceneSize.height < m_viewportSize.height ?
              (m_viewportSize.height - m_sceneSize.height) / 2 : 0;
}

shapes::Point2D Camera::calculateCameraFocalOffset() const
{
   shapes::Point2D cameraFocalOffset = shapes::Point2D::ORIGIN;
   if (m_focalPointSet)
   {
      if(m_offset.x == 0)
      {
         // Offset the camera's focal point by half the screen width to
         // center the focal point horizontally
         cameraFocalOffset.x = m_focalPoint.x - (m_viewportSize.width / 2);
         cameraFocalOffset.x = std::max(cameraFocalOffset.x, 0);
         
         const signed int rightCameraBoundary = static_cast<signed int>(m_sceneSize.width - m_viewportSize.width);
         cameraFocalOffset.x = -std::min(cameraFocalOffset.x, rightCameraBoundary);
      }
      
      if(m_offset.y == 0)
      {
         // Offset the camera's focal point by half the screen height to
         // center the focal point vertically
         cameraFocalOffset.y = m_focalPoint.y - (m_viewportSize.height / 2);
         cameraFocalOffset.y = std::max(cameraFocalOffset.y, 0);
         
         const signed int bottomCameraBoundary = static_cast<signed int>(m_sceneSize.height - m_viewportSize.height);
         cameraFocalOffset.y = -std::min(cameraFocalOffset.y, bottomCameraBoundary);
      }
   }
   
   return cameraFocalOffset;
}

void Camera::apply()
{
   if(!m_cameraApplied)
   {
      // Apply an absolute offset (to center all the drawn elements)
      GraphicsUtil::getInstance()->setAbsoluteOffset(m_offset.x, m_offset.y);
      glPushMatrix();

      // Create a scissor region around the camera's view bounds
      // To prevent any elements from being drawn outside the camera's specified view.
      glPushAttrib(GL_SCISSOR_BIT);
      glEnable(GL_SCISSOR_TEST);

      const int scissorYOffset = GraphicsUtil::getInstance()->getHeight() - m_viewportSize.height;
      glScissor(0, scissorYOffset, m_viewportSize.width, m_viewportSize.height);

      shapes::Point2D cameraFocalOffset = calculateCameraFocalOffset();

      if(cameraFocalOffset != shapes::Point2D::ORIGIN)
      {
         // Perform an inverse translation from the focal point to shift
         // the scene to the camera
         glTranslated(cameraFocalOffset.x, cameraFocalOffset.y, 0.0);
      }

      m_cameraApplied = true;
   }
}

void Camera::reset()
{
   if(m_cameraApplied)
   {
      // Reset the scissor attribute
      glDisable(GL_SCISSOR_TEST);
      glPopAttrib();

      // Reset the camera translation
      glPopMatrix();

      GraphicsUtil::getInstance()->resetAbsoluteOffset();
      m_cameraApplied = false;
   }
}
