/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "ScreenTexture.h"
#include "GraphicsUtil.h"
#include "OpenGLExtensions.h"
#include "Size.h"

#include "DebugUtils.h"
const int debugFlag = DEBUG_GRAPHICS;

ScreenTexture::ScreenTexture()
{
   m_size = shapes::Size(GraphicsUtil::getInstance()->getWidth(), GraphicsUtil::getInstance()->getHeight());
   glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);
   bind();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.width, m_size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

   glPopAttrib();

   const OpenGLExtensions& extensions = GraphicsUtil::getInstance()->getExtensions();
   if(extensions.isFrameBuffersEnabled())
   {
      (*extensions.getGenFramebuffersFunction())(1, &m_frameBuffer);
   }
}

void ScreenTexture::startCapture()
{
   const OpenGLExtensions& extensions = GraphicsUtil::getInstance()->getExtensions();
   if(extensions.isFrameBuffersEnabled())
   {
      (*extensions.getBindFramebufferFunction())(GL_FRAMEBUFFER_EXT, m_frameBuffer);
      (*extensions.getFramebufferTexture2DFunction())(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_textureHandle, 0);
   }
}

void ScreenTexture::endCapture()
{
   const OpenGLExtensions& extensions = GraphicsUtil::getInstance()->getExtensions();
   if(extensions.isFrameBuffersEnabled())
   {
      (*extensions.getBindFramebufferFunction())(GL_FRAMEBUFFER_EXT, 0);
   }

   m_valid = true;
}

ScreenTexture::~ScreenTexture()
{
   const OpenGLExtensions& extensions = GraphicsUtil::getInstance()->getExtensions();
   if(extensions.isFrameBuffersEnabled())
   {
      (*extensions.getDeleteFramebuffersFunction())(1, &m_frameBuffer);
   }
}

