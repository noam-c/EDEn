/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "BlendTransition.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_GRAPHICS

BlendTransition::BlendTransition(Texture&& oldStateTexture, Texture&& newStateTexture, long transitionLength) :
   Transition(std::move(oldStateTexture), std::move(newStateTexture), transitionLength)
{
}

void BlendTransition::draw()
{
   const float width = static_cast<float>(GraphicsUtil::getInstance()->getWidth());
   const float height = static_cast<float>(GraphicsUtil::getInstance()->getHeight());

   glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);

   m_oldStateTexture.bind();
   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   m_newStateTexture.bind();
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f(1.0f, 1.0f, 1.0f, m_progress);

   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glPopAttrib();
}
