/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "OpenGLExtensions.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

void OpenGLExtensions::initialize()
{
   GLenum err = glewInit();
   if(err)
   {
      T_T(reinterpret_cast<const char*>(glewGetErrorString(err)));
   }

   m_bufferObjectsEnabled = initBufferObjects();
   m_framebuffersEnabled = initFramebuffers();
}

bool OpenGLExtensions::initFramebuffers()
{
   if(GLEW_ARB_framebuffer_object)
   {
      m_glGenFramebuffers = ::glGenFramebuffers;
      m_glDeleteFramebuffers = ::glDeleteFramebuffers;
      m_glBindFramebuffer = ::glBindFramebuffer;
      m_glFramebufferTexture2D = ::glFramebufferTexture2D;

      return true;
   }

   if(GLEW_EXT_framebuffer_object)
   {
      m_glGenFramebuffers = ::glGenFramebuffersEXT;
      m_glDeleteFramebuffers = ::glDeleteFramebuffersEXT;
      m_glBindFramebuffer = ::glBindFramebufferEXT;
      m_glFramebufferTexture2D = ::glFramebufferTexture2DEXT;

      return true;
   }
   
   return false;
}

bool OpenGLExtensions::initBufferObjects()
{
   if(GLEW_VERSION_1_5)
   {
      m_glGenBuffers = ::glGenBuffers;
      m_glDeleteBuffers = ::glDeleteBuffers;
      m_glBindBuffer = ::glBindBuffer;
      m_glBufferData = ::glBufferData;
      
      return true;
   }

   if(GLEW_ARB_vertex_buffer_object)
   {
      m_glGenBuffers = ::glGenBuffersARB;
      m_glDeleteBuffers = ::glDeleteBuffersARB;
      m_glBindBuffer = ::glBindBufferARB;
      m_glBufferData = ::glBufferDataARB;

      return true;
   }

   return false;
}

bool OpenGLExtensions::isBufferObjectsEnabled() const
{
   return m_bufferObjectsEnabled;
}

bool OpenGLExtensions::isFrameBuffersEnabled() const
{
   return m_framebuffersEnabled;
}