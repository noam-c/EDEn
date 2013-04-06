/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "BlendState.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

BlendState::BlendState(GameContext& gameContext, GameState* oldState, GameState* newState, long transitionLength) :
   TransitionState(gameContext, "BlendState", oldState, newState, transitionLength)
{
}

BlendState::~BlendState()
{
}

void BlendState::draw()
{
   const float width = static_cast<float>(GraphicsUtil::getInstance()->getWidth());
   const float height = static_cast<float>(GraphicsUtil::getInstance()->getHeight());

   glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);

   oldStateTexture.bind();
   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

   newStateTexture.bind();
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glColor4f(1.0f, 1.0f, 1.0f, progress);

   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glPopAttrib();
}
