/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#include "SpinState.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include <cmath>

#include "DebugUtils.h"

const float PI = 3.14159265f;

const int debugFlag = DEBUG_GRAPHICS;

SpinState::SpinState(GameContext& gameContext, Texture&& oldStateTexture, long transitionLength) :
   TransitionState(gameContext, "SpinState", std::move(oldStateTexture), transitionLength)
{
}

void SpinState::draw()
{
   const float width = static_cast<float>(GraphicsUtil::getInstance()->getWidth());
   const float height = static_cast<float>(GraphicsUtil::getInstance()->getHeight());

   glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT);

   glEnable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   m_oldStateTexture.bind();

   // Warp the standard cosine curve by the progress through the transition, which will produce
   // a gradual increase in amplitude
   float scaleFactor = cos(m_progress * PI) * m_progress + 1.0f;

   // Keep the setup matrix in tact (if changes are applied elsewhere)
   glPushMatrix();

   // Move to the center of the screen
   glTranslatef(width/2.0f, height/2.0f, 0.0f);

   // Apply scaling
   glScalef(scaleFactor, scaleFactor, 1.0f);

   // Rotate according to progress through the transition
   glRotatef(m_progress * m_progress * m_transitionLength * 0.25f, 0.0f, 0.0f, 1.0f);

   // Translate back to top,left to draw texture as expected
   glTranslatef(-width/2.0f, -height/2.0f, 0.0f);

   // Draw
   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glPopMatrix();

   glDisable(GL_TEXTURE_2D);
   glPopAttrib();
}
