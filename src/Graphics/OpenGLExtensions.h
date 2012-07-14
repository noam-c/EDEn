#ifndef OPENGL_EXTENSIONS
#define OPENGL_EXTENSIONS

#define GL_GLEXT_PROTOTYPES 1
#include "SDL_opengl.h"
#undef GL_GLEXT_PROTOTYPES

class OpenGLExtensions
{
   typedef void (APIENTRY * glBindFramebufferFunction)(GLenum, GLuint);
   typedef void (APIENTRY * glDeleteFramebuffersFunction)(GLsizei, const GLuint *);
   typedef void (APIENTRY * glGenFramebuffersFunction)(GLsizei, GLuint *);
   typedef void (APIENTRY * glFramebufferTexture2DFunction)(GLenum, GLenum, GLenum, GLuint, GLint);

   glBindFramebufferFunction _glBindFramebufferEXT;
   glDeleteFramebuffersFunction _glDeleteFramebuffersEXT;
   glGenFramebuffersFunction _glGenFramebuffersEXT;
   glFramebufferTexture2DFunction _glFramebufferTexture2DEXT;

   bool framebuffersEnabled;

   public:
      OpenGLExtensions();
      /**
       * Initializes OpenGL extensions needed by the game
       */
      void initialize();
      bool isFrameBuffersEnabled() const;
      glBindFramebufferFunction getBindFramebufferFunction() const;
      glDeleteFramebuffersFunction getDeleteFramebuffersFunction() const;
      glGenFramebuffersFunction getGenFramebuffersFunction() const;
      glFramebufferTexture2DFunction getFramebufferTexture2DFunction() const;
};

#endif