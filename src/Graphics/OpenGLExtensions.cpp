/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "OpenGLExtensions.h"
#include "SDL.h"

OpenGLExtensions::OpenGLExtensions() : 
   _glBindFramebufferEXT(NULL),
   _glDeleteFramebuffersEXT(NULL),
   _glGenFramebuffersEXT(NULL),
   _glFramebufferTexture2DEXT(NULL),
   framebuffersEnabled(false)
{
}

void OpenGLExtensions::initialize()
{
   _glBindFramebufferEXT = reinterpret_cast<glBindFramebufferFunction>(SDL_GL_GetProcAddress("glBindFramebufferEXT"));
   _glDeleteFramebuffersEXT = reinterpret_cast<glDeleteFramebuffersFunction>(SDL_GL_GetProcAddress("glDeleteFramebuffersEXT"));
   _glGenFramebuffersEXT = reinterpret_cast<glGenFramebuffersFunction>(SDL_GL_GetProcAddress("glGenFramebuffersEXT"));
   _glFramebufferTexture2DEXT = reinterpret_cast<glFramebufferTexture2DFunction>(SDL_GL_GetProcAddress("glFramebufferTexture2DEXT"));

   framebuffersEnabled = 
      _glBindFramebufferEXT != NULL &&
      _glDeleteFramebuffersEXT != NULL &&
      _glGenFramebuffersEXT != NULL &&
      _glFramebufferTexture2DEXT != NULL;
}

bool OpenGLExtensions::isFrameBuffersEnabled() const
{
   return framebuffersEnabled;
}

OpenGLExtensions::glBindFramebufferFunction OpenGLExtensions::getBindFramebufferFunction() const
{
   return _glBindFramebufferEXT;
}

OpenGLExtensions::glDeleteFramebuffersFunction OpenGLExtensions::getDeleteFramebuffersFunction() const
{
   return _glDeleteFramebuffersEXT;
}

OpenGLExtensions::glGenFramebuffersFunction OpenGLExtensions::getGenFramebuffersFunction() const
{
   return _glGenFramebuffersEXT;
}

OpenGLExtensions::glFramebufferTexture2DFunction OpenGLExtensions::getFramebufferTexture2DFunction() const
{
   return _glFramebufferTexture2DEXT;
}
