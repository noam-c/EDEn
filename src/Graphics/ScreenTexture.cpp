/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
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
   auto graphics = GraphicsUtil::getInstance();
   auto& extensions = graphics->getExtensions();

   glGenTextures(1, &m_textureHandle);
   m_size = geometry::Size(graphics->getWidth(), graphics->getHeight());
   glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);
   bind();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_size.width, m_size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

   glPopAttrib();

   extensions.glGenFramebuffers(1, &m_frameBuffer);
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
   
   GraphicsUtil::getInstance()->getExtensions().glDeleteFramebuffers(1, &m_frameBuffer);
}

ScreenTexture ScreenTexture::create(GameState& gameState)
{
   ScreenTexture texture;

   auto& extensions = GraphicsUtil::getInstance()->getExtensions();
   if(extensions.isFrameBuffersEnabled())
   {
      bool wasActive = gameState.isActive();
      
      if(!wasActive)
      {
         gameState.activate();
      }

      extensions.glBindFramebuffer(GL_FRAMEBUFFER, texture.m_frameBuffer);
      extensions.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture.m_textureHandle, 0);
      
      gameState.drawFrame();

      extensions.glBindFramebuffer(GL_FRAMEBUFFER, 0);

      if(!wasActive)
      {
         gameState.deactivate();
      }

      texture.m_valid = true;
   }

   return texture;
}
