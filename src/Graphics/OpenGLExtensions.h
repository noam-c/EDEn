/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef OPENGL_EXTENSIONS
#define OPENGL_EXTENSIONS

#include <GL/glew.h>

// Boilerplate for OpenGL extensions.
// Declares a member function pointer and creates a
// public function to forward calls to it.
// \todo: This is a helpful way to reduce code repetition,
// but makes debugging kind of a pain. Replace this with
// code generation once such a solution is implemented.
#define EXTENSION(ext) \
  private: \
    decltype(ext) m_##ext = nullptr; \
  public: \
    template<typename ... Args> auto ext(Args... args) \
      -> decltype(ext(args...)) \
    { \
       m_##ext(args...); \
    }

/**
 * A helper class to select the extensions needed
 * by the game based on what's available in the
 * underlying hardware.
 *
 * @author Noam Chitayat
 */
class OpenGLExtensions final
{
   
   /**
    * Flag indicating whether or not Frame Buffer Objects (FBOs) are supported on this device.
    */
   bool m_framebuffersEnabled;

   /**
    * Flag indicating whether or not Buffer Objects are supported on this device.
    */
   bool m_bufferObjectsEnabled;
   
   /* Framebuffers */

   EXTENSION(glGenFramebuffers);
   EXTENSION(glDeleteFramebuffers);
   EXTENSION(glBindFramebuffer);
   EXTENSION(glFramebufferTexture2D);

   /* Buffer objects */
   EXTENSION(glGenBuffers);
   EXTENSION(glDeleteBuffers);
   EXTENSION(glBindBuffer);
   EXTENSION(glBufferData);
   
   bool initFramebuffers();
   
   bool initBufferObjects();

   public:
      /**
       * Initializes OpenGL extensions needed by the game
       */
      void initialize();

      /**
       * @return true iff buffer objects are supported on this device.
       */
      bool isBufferObjectsEnabled() const;

      /**
       * @return true iff FBOs are supported on this device.
       */
      bool isFrameBuffersEnabled() const;
};

#undef EXTENSION

#endif
