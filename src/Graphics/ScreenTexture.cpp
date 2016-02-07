/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "ScreenTexture.h"
#include "GameState.h"
#include "GraphicsUtil.h"
#include "Size.h"

#include "DebugUtils.h"
#define DEBUG_FLAG DEBUG_GRAPHICS

ScreenTexture::ScreenTexture() :
   m_frameBuffer(0)
{
   glGenTextures(1, &m_textureHandle);
   m_size = geometry::Size(GraphicsUtil::getInstance()->getWidth(), GraphicsUtil::getInstance()->getHeight());
   glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);
   bind();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.width, m_size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

   glPopAttrib();

   if(GLEW_ARB_framebuffer_object)
   {
      glGenFramebuffers(1, &m_frameBuffer);
   }
   else if(GLEW_EXT_framebuffer_object)
   {
      glGenFramebuffersEXT(1, &m_frameBuffer);
   }
}

ScreenTexture::ScreenTexture(ScreenTexture&& rhs) :
   Texture(std::move(rhs)),
   m_frameBuffer(0)
{
   if(m_valid)
   {
      std::swap(m_frameBuffer, rhs.m_frameBuffer);
   }
}

ScreenTexture& ScreenTexture::operator=(ScreenTexture&& rhs)
{
   Texture::operator=(std::move(rhs));
   if(m_valid)
   {
      m_frameBuffer = rhs.m_frameBuffer;
      rhs.m_frameBuffer = 0;
   }
   
   return *this;
}

ScreenTexture::~ScreenTexture()
{
   if(!m_valid)
   {
      return;
   }
   
   if(GLEW_ARB_framebuffer_object)
   {
      glDeleteFramebuffers(1, &m_frameBuffer);
   }
   else if(GLEW_EXT_framebuffer_object)
   {
      glDeleteFramebuffersEXT(1, &m_frameBuffer);
   }
}

ScreenTexture ScreenTexture::create(GameState& gameState)
{
   ScreenTexture texture;

   if(GLEW_EXT_framebuffer_object || GLEW_ARB_framebuffer_object)
   {
      bool wasActive = gameState.isActive();
      
      if(!wasActive)
      {
         gameState.activate();
      }

      if(GLEW_ARB_framebuffer_object)
      {
         glBindFramebuffer(GL_FRAMEBUFFER, texture.m_frameBuffer);
         glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.m_textureHandle, 0);
         
         glBindFramebuffer(GL_FRAMEBUFFER, 0);
      }
      else if(GLEW_EXT_framebuffer_object)
      {
         glBindFramebuffer(GL_FRAMEBUFFER_EXT, texture.m_frameBuffer);
         glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture.m_textureHandle, 0);

         glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
      }
      

      if(!wasActive)
      {
         gameState.deactivate();
      }

      texture.m_valid = true;
   }

   return texture;
}
