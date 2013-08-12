/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef OPENGL_EXTENSIONS
#define OPENGL_EXTENSIONS

#define GL_GLEXT_PROTOTYPES 1
#include "SDL_opengl.h"
#undef GL_GLEXT_PROTOTYPES

/**
 * A helper class to load in and manage the extensions needed by the game.
 *
 * @author Noam Chitayat
 */
class OpenGLExtensions
{
   typedef void (APIENTRY * glBindFramebufferFunction)(GLenum, GLuint);
   typedef void (APIENTRY * glDeleteFramebuffersFunction)(GLsizei, const GLuint *);
   typedef void (APIENTRY * glGenFramebuffersFunction)(GLsizei, GLuint *);
   typedef void (APIENTRY * glFramebufferTexture2DFunction)(GLenum, GLenum, GLenum, GLuint, GLint);

   /**
    * The pointer to the glBindFramebufferEXT function.
    */
   glBindFramebufferFunction m_glBindFramebufferEXT;

   /**
    * The pointer to the glDeleteFramebuffersEXT function.
    */
   glDeleteFramebuffersFunction m_glDeleteFramebuffersEXT;

   /**
    * The pointer to the glGenFramebuffersEXT function.
    */
   glGenFramebuffersFunction m_glGenFramebuffersEXT;

   /**
    * The pointer to the glFramebufferTexture2DEXT function.
    */
   glFramebufferTexture2DFunction m_glFramebufferTexture2DEXT;

   /**
    * Flag indicating whether or not Frame Buffer Objects (FBOs) are supported on this device.
    */
   bool m_framebuffersEnabled;

   public:

      /**
       * Constructor.
       */
      OpenGLExtensions();

      /**
       * Initializes OpenGL extensions needed by the game
       */
      void initialize();

      /**
       * @return true iff FBOs are supported on this device.
       */
      bool isFrameBuffersEnabled() const;

      /**
       * @return a pointer to the glBindFramebufferEXT function.
       */
      glBindFramebufferFunction getBindFramebufferFunction() const;

      /**
       * @return a pointer to the glDeleteFramebuffersEXT function.
       */
      glDeleteFramebuffersFunction getDeleteFramebuffersFunction() const;

      /**
       * @return a pointer to the glGenFramebuffersEXT function.
       */
      glGenFramebuffersFunction getGenFramebuffersFunction() const;

      /**
       * @return a pointer to the glFramebufferTexture2DEXT function.
       */
      glFramebufferTexture2DFunction getFramebufferTexture2DFunction() const;
};

#endif
