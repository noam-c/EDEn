/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2015 Noam Chitayat. All rights reserved.
 */

#ifndef SCREEN_TEXTURE_H
#define SCREEN_TEXTURE_H

typedef unsigned int GLuint;

#include "Texture.h"

class GameState;

/**
 * A texture created from a capture of the screen's state.
 * Call startCapture to redirect drawing operations to this
 * ScreenTexture's FBO, draw normally, and then call endCapture
 * to capture the drawn contents in this object's texture.
 *
 * Allows for drawing a screen capture as a texture on an object,
 * enabling visual transformations without requiring numerous redraws
 * of the screen.
 *
 * A ScreenTexture's methods are mostly no-ops when FBOs are not available
 * on the device.
 *
 * @author Noam Chitayat
 * @author Bobby Richter
 */
class ScreenTexture : public Texture
{
   protected:
      /**
       * The frame buffer to draw to when recording the screen contents.
       */
      GLuint m_frameBuffer;

   public:
      /**
       * Constructor.
       * Initializes the FBO used to record the OpenGL drawing operations.
       */
      ScreenTexture();

      /**
       * Copying is disallowed.
       */
      ScreenTexture(const ScreenTexture& rhs) = delete;

      /**
       * Move constructor and assignment.
       */
      ScreenTexture(ScreenTexture&& rhs);
      ScreenTexture& operator=(ScreenTexture&& rhs);

      /**
       * Destructor.
       * Cleans up the allocated FBO.
       */
      ~ScreenTexture();

      /**
       * Takes a snapshot of the given GameState and
       * captures it into the texture.
       */
      static ScreenTexture create(GameState& gameState);
};

#endif
