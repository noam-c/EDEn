/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "GraphicsUtil.h"
#include <SDL.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "guichan.hpp"
#include "guichan/sdl.hpp"
#include "guichan/opengl.hpp"
#include "guichan/opengl/openglsdlimageloader.hpp"
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include "RocketSDLInputMapping.h"
#include "Container.h"
#include "Size.h"
#include "OpenGLTTF.h"
#include <dirent.h>

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

SDL_Surface* GraphicsUtil::screen = NULL;

void GraphicsUtil::initialize()
{
   currentXOffset = 0;
   currentYOffset = 0;

   initSDL();
   openGLExtensions.initialize();
   initRocket();
   initGuichan();
}

void GraphicsUtil::initSDL()
{
   // Audio information (HARDCODED)
   int audio_rate = 44100;
   Uint16 audio_format = AUDIO_S16SYS; /* 16-bit stereo */
   int audio_channels = 2;
   int audio_buffers = 2048;

   // Initialize SDL audio and video bindings
   if(SDL_Init (SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
   {
      printf ("Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit(1);
   }

   // Initialize SDL_mixer audio library
   if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
   {
      printf("Unable to open audio: %s\n", SDL_GetError());
      exit(1);
   }

   // Enable the OpenGL double buffer
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

   // On exit, run the SDL cleanup
   atexit (SDL_Quit);
 
   // Set 800x600 32-bits video mode (HARDCODED)
   screen = SDL_SetVideoMode (width, height, 32, SDL_HWSURFACE | SDL_OPENGL | SDL_HWACCEL);
   if(screen == NULL)
   {
      printf ("Couldn't set 800x600x32 video mode: %s\n", SDL_GetError());
      exit(1);
   }

   // Enable Texture Mapping
   glEnable(GL_TEXTURE_2D);

   // Sprites drawn to screen replace whatever is behind them (tiles, background)
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

   // Set up the viewport and reset the projection matrix
   glViewport(0, 0, width, height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   // Set the clear color to black
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   // Create a 2D orthogonal perspective (better for 2D games)
   gluOrtho2D(0.0f, (float)width, (float)height, 0.0f);

   //Initialize SDL_ttf for use of TrueType Fonts
   if(TTF_Init() == -1)
   {
      printf("TTF_Init: %s\n", TTF_GetError());
      exit(1);
   }

   // We want unicode
   SDL_EnableUNICODE(1);
   // We want to enable key repeat
   SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

   SDL_WM_SetCaption ("Buhr Prototype", NULL);
}

void GraphicsUtil::initRocket()
{
    Rocket::Core::SetSystemInterface(&rocketSystemInterface);
    Rocket::Core::SetRenderInterface(&rocketRenderInterface);
    Rocket::Core::Initialise();
    Rocket::Controls::Initialise();

    const std::string fontLocation = "data/fonts";
    struct dirent *entry;
    DIR *dp;
    dp = opendir(fontLocation.c_str());
    if (dp == NULL)
    {
       T_T("Failed to open data/fonts for font loading.");
    }

    while((entry = readdir(dp)))
    {
       const std::string filename = entry->d_name;
       if(filename.length() > 4)
       {
          const std::string extension = filename.substr(filename.length() - 4, 4);

          if(extension == ".ttf" || extension == ".otf")
          {
             const std::string path = fontLocation + '/' + entry->d_name;
             if(!Rocket::Core::FontDatabase::LoadFontFace(path.c_str()))
             {
                DEBUG("Failed to load font: %s", path.c_str());
             }
          }
       }
    }

    closedir(dp);

    RocketSDLInputMapping::initialize();
}

void GraphicsUtil::initGuichan()
{
   imageLoader = new gcn::OpenGLSDLImageLoader();

   // The ImageLoader in use is static and must be set to be
   // able to load images
   gcn::Image::setImageLoader(imageLoader);
   graphics = new gcn::OpenGLGraphics();
   graphics->setTargetPlane(800, 600);

   input = new gcn::SDLInput();

   gui = new gcn::Gui();

   // Set gui to use the SDLGraphics object.
   gui->setGraphics(graphics);

   // Set gui to use the SDLInput object
   gui->setInput(input);

   // Set top-level container for the gui
   guiContainer = new edwt::Container();
   gui->setTop(guiContainer);
   guiContainer->setWidth(width);
   guiContainer->setHeight(height);
   guiContainer->setVisible(true);
   guiContainer->setOpaque(false);
   
   // Set the top-level container to be null for now
   top = NULL;

   // Load the image font.
   font = new edwt::OpenGLTrueTypeFont("data/fonts/LDSRegular.ttf", 16);

   // The global font is static and must be set.
   gcn::Widget::setGlobalFont(font);
}

void GraphicsUtil::flipScreen()
{
   glFlush();
   SDL_GL_SwapBuffers();
}

const OpenGLExtensions& GraphicsUtil::getExtensions() const
{
   return openGLExtensions;
}

int GraphicsUtil::getWidth()
{
   return width;
}

int GraphicsUtil::getHeight()
{
   return height;
}

void GraphicsUtil::setInterface(gcn::Container* newTop)
{
   if(top != NULL)
   {
      top->setVisible(false);
   }
   
   if(newTop != NULL)
   {
      if(guiContainer->containsWidget(newTop))
      {
         newTop->setVisible(true);
      }
      else
      {
         guiContainer->add(newTop);
      }
   }

   top = newTop;
}

void GraphicsUtil::stepGUI()
{
   gui->logic();
}

void GraphicsUtil::drawGUI()
{
   // Draw the GUI to buffer
   gui->draw();

   // Update the screen
   SDL_GL_SwapBuffers();
}

void GraphicsUtil::pushInput(const SDL_Event& event)
{
   input->pushInput(event);
}

void GraphicsUtil::clearBuffer()
{
   glMatrixMode(GL_MODELVIEW);
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();
}

void GraphicsUtil::setOffset(int xOffset, int yOffset)
{
   glTranslated(xOffset - currentXOffset, yOffset - currentYOffset, 0);
   currentXOffset = xOffset;
   currentYOffset = yOffset;
}

void GraphicsUtil::resetOffset()
{
   glTranslated(-currentXOffset, -currentYOffset, 0);
   currentXOffset = 0;
   currentYOffset = 0;
}

void GraphicsUtil::FadeToColor(float red, float green, float blue, int delay)
{
   long time = SDL_GetTicks();
   float alpha = 0.0f;

   glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);
   glDisable(GL_DEPTH_TEST);
   glDisable(GL_TEXTURE_2D);

   for (;;)
   {
      glBegin(GL_QUADS);
         glColor4f(red, green, blue, alpha);
         glVertex3f( 0.0f,         0.0f,          0.0f);
         glVertex3f( (float)width, 0.0f,          0.0f);
         glVertex3f( (float)width, (float)height, 0.0f);
         glVertex3f( 0.0f,         (float)height, 0.0f);
      glEnd();

      GraphicsUtil::getInstance()->flipScreen();
 
      long timePassed = SDL_GetTicks() - time;
      alpha = ((timePassed*1.0f)/delay);

      //We're done when alpha reaches 1.0
      if (alpha >= 1.0f)
      {
         glColor3f(1.0f,1.0f,1.0f);
         break;
      }
   }

   glPopAttrib();
}

void GraphicsUtil::finish()
{
   //Destroys some Guichan stuff
   delete font;
   delete guiContainer;
   delete gui;
   delete imageLoader;
   delete graphics;
   delete input;

   //Destroy the SDL_ttf stuff
   if(!TTF_WasInit())
   {
      TTF_Quit();
   }

   // Destroy SDL stuff
   SDL_Quit();
}
