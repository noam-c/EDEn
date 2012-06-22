/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "FadeState.h"
#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

FadeState::FadeState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength)
   : GameState(executionStack), oldState(oldState), newState(newState), startTime(SDL_GetTicks()), transitionLength(transitionLength), alpha(1.0f), transitionComplete(false)
{
   DEBUG("Creating fade state.");
   screenTexture.startCapture();
   oldState->drawFrame();
   screenTexture.endCapture();

   if(glGetError())
   {
      DEBUG("Failed to create screen capture.");
   }
}

bool FadeState::step()
{
   if(transitionComplete)
   {
      return false;
   }

   long timePassed = SDL_GetTicks() - startTime;

   if(timePassed > transitionLength)
   {
      DEBUG("Finishing fade.");
      executionStack.pushState(newState);
      transitionComplete = true;
   }

   alpha = static_cast<double>(timePassed) / static_cast<double>(transitionLength);
   DEBUG("Continuing fade (%dms passed).", timePassed);
   return true;
}

void FadeState::draw()
{
   const float width = static_cast<float>(GraphicsUtil::getInstance()->getWidth());
   const float height = static_cast<float>(GraphicsUtil::getInstance()->getHeight());

   glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

   glLoadIdentity();
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   screenTexture.bind();

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
      glColor4f(0.0f, 0.0f, 0.0f, alpha);
      glVertex3f(0.0f, 0.0f, 0.0f);
      glVertex3f(width, 0.0f, 0.0f);
      glVertex3f(width, height, 0.0f);
      glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glPopAttrib();
}

FadeState::~FadeState()
{
}
