#include "ScreenTexture.h"
#include "GraphicsUtil.h"
#include "Size.h"

#define GL_GLEXT_PROTOTYPES 1
#include "SDL_opengl.h"
#undef GL_GLEXT_PROTOTYPES

#include "DebugUtils.h"
const int debugFlag = DEBUG_GRAPHICS;

ScreenTexture::ScreenTexture()
{
   size = shapes::Size(GraphicsUtil::getInstance()->getWidth(), GraphicsUtil::getInstance()->getHeight());
   glPushAttrib(GL_ENABLE_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);
   bind();

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width, size.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

   glPopAttrib();
}

void ScreenTexture::startCapture()
{
   glGenFramebuffersEXT(1, &frameBuffer);
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, textureHandle, 0);
}

void ScreenTexture::endCapture()
{
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

ScreenTexture::~ScreenTexture()
{
   glDeleteFramebuffersEXT(1, &frameBuffer);
}

