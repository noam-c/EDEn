/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "OpenGLExtensions.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_GRAPHICS

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
      m_GenFramebuffers = ::glGenFramebuffers;
      m_DeleteFramebuffers = ::glDeleteFramebuffers;
      m_BindFramebuffer = ::glBindFramebuffer;
      m_FramebufferTexture2D = ::glFramebufferTexture2D;

      return true;
   }

   if(GLEW_EXT_framebuffer_object)
   {
      m_GenFramebuffers = ::glGenFramebuffersEXT;
      m_DeleteFramebuffers = ::glDeleteFramebuffersEXT;
      m_BindFramebuffer = ::glBindFramebufferEXT;
      m_FramebufferTexture2D = ::glFramebufferTexture2DEXT;

      return true;
   }
   
   return false;
}

bool OpenGLExtensions::initBufferObjects()
{
   if(GLEW_VERSION_1_5)
   {
      m_GenBuffers = ::glGenBuffers;
      m_DeleteBuffers = ::glDeleteBuffers;
      m_BindBuffer = ::glBindBuffer;
      m_BufferData = ::glBufferData;
      
      return true;
   }

   if(GLEW_ARB_vertex_buffer_object)
   {
      m_GenBuffers = ::glGenBuffersARB;
      m_DeleteBuffers = ::glDeleteBuffersARB;
      m_BindBuffer = ::glBindBufferARB;
      m_BufferData = ::glBufferDataARB;

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
