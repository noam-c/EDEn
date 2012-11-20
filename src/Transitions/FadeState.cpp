/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "FadeState.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_TRANSITIONS;

FadeState::FadeState(ExecutionStack& executionStack, GameState* oldState, long transitionLength) :
   TransitionState(executionStack, "FadeState", oldState, NULL, transitionLength)
{
}

FadeState::~FadeState()
{
}

void FadeState::draw()
{
   const float width = static_cast<float>(GraphicsUtil::getInstance()->getWidth());
   const float height = static_cast<float>(GraphicsUtil::getInstance()->getHeight());

   glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   oldStateTexture.bind();

   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glDisable(GL_TEXTURE_2D);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

   glBegin(GL_QUADS);
      glColor4f(0.0f, 0.0f, 0.0f, progress);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(width, 0.0f, 0.0f);
      glVertex3f(width, height, 0.0f);
      glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glPopAttrib();
}
