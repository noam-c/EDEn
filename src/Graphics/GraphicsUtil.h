/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#ifndef GRAPHICS_UTIL_H
#define GRAPHICS_UTIL_H

#include "Singleton.h"

#include "OpenGLExtensions.h"
#include "RocketContextRegistry.h"
#include "EdenRocketRenderInterface.h"
#include "EdenRocketSystemInterface.h"

class Settings;

struct SDL_Window;
typedef void* SDL_GLContext;
union SDL_Event;

namespace shapes
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
class GraphicsUtil : public Singleton<GraphicsUtil>
{
   /** The main window */
   SDL_Window* window;

   /** The OpenGL context used by the main window */
   SDL_GLContext openGLContext;

   /** The OpenGL Extensions */
   OpenGLExtensions openGLExtensions;

   /** The render interface that Rocket will use. */
   EdenRocketRenderInterface rocketRenderInterface;

   /** The system interface that Rocket will use. */
   EdenRocketSystemInterface rocketSystemInterface;

   /** The registry used to track all the contexts for resolution changes. */
   RocketContextRegistry rocketContextRegistry;
   
   bool fullScreenEnabled;
   
   /** The screen width */
   unsigned int width;
   
   /** The screen height */
   unsigned int height;
   
   /** The bit depth of the screen buffer */
   unsigned int bitsPerPixel;
   
   /** The x-offset to draw at (in pixels). */
   int currentXOffset;

   /** The y-offset to draw at (in pixels). */
   int currentYOffset;

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
   bool initSDLVideoMode(std::string*& errorMsg);

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
      virtual void initialize();
   
      /**
       * Singleton destructor.
       * Cleans up the following:
       * - drivers
       * - global font
       * - the SDL TrueTypeFont library
       * - the SDL layer
       */
      virtual void finish();

   public:
   
      bool isVideoModeRefreshRequired() const;
      bool refreshVideoMode(std::string*& errorMsg);
   
      /**
       * @return The extension manager for this graphical context.
       */
      const OpenGLExtensions& getExtensions() const;

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
      Rocket::Core::Context* createRocketContext(const std::string& name);

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
       *  Graphical effect: Fade to a specific colour in a specified time period
       *
       *  @param red   The amount of red   (0.0f <= red <= 1.0f)
       *  @param green The amount of green (0.0f <= green <= 1.0f)
       *  @param blue  The amount of blue  (0.0f <= blue <= 1.0f)
       *  @param delay The amount of time taken for the fade (in milliseconds)
       */
      void FadeToColor(float red, float green, float blue, int delay);
   
      /**
       * Clear the color buffers and reset the model matrix
       * \todo Come up with a better name for this (though that suggestion indicates that this method may not be good design).
       */
      void clearBuffer();
};

#endif

