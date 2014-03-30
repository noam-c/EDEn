/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "OpenGLExtensions.h"
#include "SDL.h"

OpenGLExtensions::OpenGLExtensions() : 
   m_glBindFramebufferEXT(nullptr),
   m_glDeleteFramebuffersEXT(nullptr),
   m_glGenFramebuffersEXT(nullptr),
   m_glFramebufferTexture2DEXT(nullptr),
   m_framebuffersEnabled(false)
{
}

void OpenGLExtensions::initialize()
{
   m_glBindFramebufferEXT = reinterpret_cast<glBindFramebufferFunction>(SDL_GL_GetProcAddress("glBindFramebufferEXT"));
   m_glDeleteFramebuffersEXT = reinterpret_cast<glDeleteFramebuffersFunction>(SDL_GL_GetProcAddress("glDeleteFramebuffersEXT"));
   m_glGenFramebuffersEXT = reinterpret_cast<glGenFramebuffersFunction>(SDL_GL_GetProcAddress("glGenFramebuffersEXT"));
   m_glFramebufferTexture2DEXT = reinterpret_cast<glFramebufferTexture2DFunction>(SDL_GL_GetProcAddress("glFramebufferTexture2DEXT"));

   m_framebuffersEnabled =
      m_glBindFramebufferEXT != nullptr &&
      m_glDeleteFramebuffersEXT != nullptr &&
      m_glGenFramebuffersEXT != nullptr &&
      m_glFramebufferTexture2DEXT != nullptr;
}

bool OpenGLExtensions::isFrameBuffersEnabled() const
{
   return m_framebuffersEnabled;
}

OpenGLExtensions::glBindFramebufferFunction OpenGLExtensions::getBindFramebufferFunction() const
{
   return m_glBindFramebufferEXT;
}

OpenGLExtensions::glDeleteFramebuffersFunction OpenGLExtensions::getDeleteFramebuffersFunction() const
{
   return m_glDeleteFramebuffersEXT;
}

OpenGLExtensions::glGenFramebuffersFunction OpenGLExtensions::getGenFramebuffersFunction() const
{
   return m_glGenFramebuffersEXT;
}

OpenGLExtensions::glFramebufferTexture2DFunction OpenGLExtensions::getFramebufferTexture2DFunction() const
{
   return m_glFramebufferTexture2DEXT;
}
