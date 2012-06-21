#include "SpinState.h"
#include "ExecutionStack.h"
#include "GraphicsUtil.h"
#include "SDL.h"
#include "SDL_opengl.h"

#include <cmath>

#include "DebugUtils.h"

const float PI = 3.14159265f;

const int debugFlag = DEBUG_GRAPHICS;

SpinState::SpinState(ExecutionStack& executionStack, GameState* oldState, GameState* newState, long transitionLength)
   : GameState(executionStack), oldState(oldState), newState(newState), startTime(SDL_GetTicks()), transitionLength(transitionLength), alpha(1.0f), transitionComplete(false)
{
   DEBUG("Creating spin state.");
   screenTexture.startCapture();
   oldState->drawFrame();
   screenTexture.endCapture();

   if(glGetError())
   {
      DEBUG("Failed to create screen capture.");
   }
}

bool SpinState::step()
{
   if(transitionComplete)
   {
      return false;
   }

   timePassed = SDL_GetTicks() - startTime;

   if(timePassed > transitionLength)
   {
      DEBUG("Finishing spin.");
      executionStack.pushState(newState);
      transitionComplete = true;
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
   screenTexture.bind();

   float scaleFactor = cos(alpha*PI) * alpha + 1.0f;
   DEBUG("Scale: %f", scaleFactor);

   glPushMatrix();
   glTranslatef(width/2.0f, height/2.0f, 0.0f);
   glScalef(scaleFactor, scaleFactor, 1.0f);
   glRotatef(alpha * timePassed * 0.25f, 0.0f, 0.0f, 1.0f);

   glPushMatrix();
   glTranslatef(-width/2.0f, -height/2.0f, 0.0f);
   glBegin(GL_QUADS);
      glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 1.0f); glVertex3f(width, 0.0f, 0.0f);
      glTexCoord2f(1.0f, 0.0f); glVertex3f(width, height, 0.0f);
      glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f, height, 0.0f);
   glEnd();

   glDisable(GL_TEXTURE_2D);
   glPopMatrix();

   glPopMatrix();
   glPopAttrib();
}

SpinState::~SpinState()
{
}
