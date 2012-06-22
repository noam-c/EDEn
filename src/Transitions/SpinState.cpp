/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "SpinState.h"
#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include <cmath>

#include "DebugUtils.h"

const float PI = 3.14159265f;

const int debugFlag = DEBUG_GRAPHICS;

SpinState::SpinState(ExecutionStack& executionStack, GameState* oldState, long transitionLength)
   : GameState(executionStack), oldState(oldState), startTime(SDL_GetTicks()), transitionLength(transitionLength), alpha(1.0f)
{
   DEBUG("Creating spin state.");

   oldState->activate();
   oldStateTexture.startCapture();
   oldState->drawFrame();
   oldStateTexture.endCapture();

   if(glGetError())
   {
      DEBUG("Failed to create screen capture for old state.");
   }

   activate();
}

bool SpinState::step()
{
   timePassed = SDL_GetTicks() - startTime;

   if(timePassed > transitionLength)
   {
      DEBUG("Finishing spin.");
      return false;
   }

   alpha = static_cast<double>(timePassed) / static_cast<double>(transitionLength);
   DEBUG("Continuing spin (%dms passed).", timePassed);
   return true;
}

void SpinState::draw()
{
   const float width = static_cast<float>(GraphicsUtil::getInstance()->getWidth());
   const float height = static_cast<float>(GraphicsUtil::getInstance()->getHeight());

   glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

   glLoadIdentity();
   glEnable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   oldStateTexture.bind();

   // Warp the standard cosine curve by the alpha value, which will produce
   // a gradual increase in amplitude
   float scaleFactor = cos(alpha*PI) * alpha + 1.0f;

   // Keep the setup matrix in tact (if changes are applied elsewhere)
   glPushMatrix();

   // Move to the center of the screen
   glTranslatef(width/2.0f, height/2.0f, 0.0f);

   // Apply scaling
   glScalef(scaleFactor, scaleFactor, 1.0f);

   // Rotate according to timePassed
   glRotatef(alpha * timePassed * 0.25f, 0.0f, 0.0f, 1.0f);

   glPushMatrix();

   // Translate back to top,left to draw texture as expected
   glTranslatef(-width/2.0f, -height/2.0f, 0.0f);

   // Draw
   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   // Pop twice, once for rotation/offset, and once more to get original matrix back
   glPopMatrix();
   glPopMatrix();

   glDisable(GL_TEXTURE_2D);
   glPopAttrib();
}

SpinState::~SpinState()
{
}
