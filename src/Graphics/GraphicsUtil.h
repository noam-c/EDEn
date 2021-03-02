/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#ifndef GRAPHICS_UTIL_H
#define GRAPHICS_UTIL_H

#include "Singleton.h"

#include "OpenGLExtensions.h"
#include "RocketContextRegistry.h"
#include "EdenRocketRenderInterface.h"
#include "EdenRocketSystemInterface.h"

#include <memory>
#include <tuple>

class Settings;

struct SDL_Window;
typedef void* SDL_GLContext;
union SDL_Event;

namespace geometry
{
   struct Size;
};

typedef unsigned int GLuint;

/**
 * All cross-class utilities for graphic functionality, such as initialization, effects, and drawing
 * are encapsulated in this singleton class.
 *
 * Note: This class is a singleton.
 *
 * @author Noam Chitayat
 */
class GraphicsUtil final : public Singleton<GraphicsUtil>
{
   /** The main window */
   SDL_Window* m_window;

   /** The OpenGL context used by the main window */
   SDL_GLContext m_openGLContext;

   /** The OpenGL Extensions */
   OpenGLExtensions m_openGLExtensions;

   /** The render interface that Rocket will use. */
   EdenRocketRenderInterface m_rocketRenderInterface;

   /** The system interface that Rocket will use. */
   EdenRocketSystemInterface m_rocketSystemInterface;

   /** The registry used to track all the contexts for resolution changes. */
   RocketContextRegistry m_rocketContextRegistry;

   bool m_fullScreenEnabled;

   /** The screen width */
   unsigned int m_width;

   /** The screen height */
   unsigned int m_height;

   /** The bit depth of the screen buffer */
   unsigned int m_bitsPerPixel;

   /** The x-offset to draw at (in pixels). */
   int m_currentXOffset;

   /** The y-offset to draw at (in pixels). */
   int m_currentYOffset;

   /**
    * Initializes SDL audio and video bindings
    * Initializes SDL mixer and TTF libraries
    * Initializes an OpenGL viewport and projection
    */
   void initSDL();

   /**
    * Initializes SDL video mode.
    *
    * @param errorMsg The error generated if setting the video mode fails.
    *
    * @return true iff setting the video mode was successful.
    */
   std::tuple<bool, std::string> initSDLVideoMode();

   /**
    * Initializes the Rocket library to render RML pages as GUI elements.
    */
   void initRocket();

   protected:
      /**
       * Constructor.
       * Initializes SDL and OpenGL.
       * Initialize Rocket GUI library.
       */
      void initialize() override;

      /**
       * Singleton destructor.
       * Cleans up the following:
       * - drivers
       * - global font
       * - the SDL TrueTypeFont library
       * - the SDL layer
       */
      void finish() override;

   public:

      bool isVideoModeRefreshRequired() const;
      std::tuple<bool, std::string> refreshVideoMode();

      /**
       * @return The extension manager for this graphical context.
       */
      OpenGLExtensions& getExtensions();
   
      /**
       * @return The width of the screen
       */
      int getWidth() const;

      /**
       * @return The height of the screen
       */
      int getHeight() const;

      /**
       * @param The name of the new Rocket context.
       *
       * @return a new Rocket context that spans the entire screen
       *         and has the specified name.
       */
      Rml::Core::Context* createRocketContext(const std::string& name);

      /**
       * @param The name of the Rocket context to remove.
       *
       * @return true iff the context was successfully removed.
       */
      bool removeRocketContext(const std::string& name);

      /**
       * Flush any enqueued GL commands and then flip the screen buffer
       */
      void flipScreen();

      /**
       * Sets a camera offset to begin drawing at. Note that this offset is
       * absolute; the offset is from the origin, rather than from the current
       * offset.
       *
       * @param xOffset The x-offset (from the origin) to draw at.
       * @param yOffset The y-offset (from the origin) to draw at.
       */
      void setAbsoluteOffset(int xOffset, int yOffset);

      /**
       * Resets the camera offset to 0.
       */
      void resetAbsoluteOffset();

      /**
       * Clear the color buffers and reset the model matrix
       * \todo Come up with a better name for this (though that suggestion indicates that this method may not be good design).
       */
      void clearBuffer();
};

#endif

