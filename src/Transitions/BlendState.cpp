/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "BlendState.h"
#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

BlendState::BlendState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength)
   : GameState(executionStack), oldState(oldState), newState(newState), startTime(SDL_GetTicks()), transitionLength(transitionLength), alpha(1.0f)
{
   DEBUG("Creating blend state.");

   oldState->activate();
   oldStateTexture.startCapture();
   oldState->drawFrame();
   oldStateTexture.endCapture();

   if(glGetError())
   {
      DEBUG("Failed to create screen capture for old state.");
   }

   newState->activate();
   newStateTexture.startCapture();
   newState->drawFrame();
   newStateTexture.endCapture();

   if(glGetError())
   {
      DEBUG("Failed to create screen capture for new state.");
   }

   activate();
}

bool BlendState::step()
{
   long timePassed = SDL_GetTicks() - startTime;

   if(timePassed > transitionLength)
   {
      DEBUG("Finishing blend.");
      return false;
   }

   alpha = static_cast<double>(timePassed) / static_cast<double>(transitionLength);
   DEBUG("Continuing blend (%dms passed).", timePassed);
   return true;
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
   glColor4f(1.0f, 1.0f, 1.0f, alpha);

   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glPopAttrib();
}

BlendState::~BlendState()
{
}
