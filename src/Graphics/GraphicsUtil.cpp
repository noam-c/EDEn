/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2013 Noam Chitayat. All rights reserved.
 */

#include "GraphicsUtil.h"
#include <SDL.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include <Rocket/Core.h>
#include <Rocket/Controls.h>
#include "RocketSDLInputMapping.h"
#include "Settings.h"
#include "Size.h"
#include <dirent.h>

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

void GraphicsUtil::initialize()
{
   m_window = nullptr;
   m_openGLContext = nullptr;
   const Settings::Resolution& resolution = Settings::getCurrentSettings().getResolution();
   m_fullScreenEnabled = Settings::getCurrentSettings().isFullScreenEnabled();
   m_width = resolution.getWidth();
   m_height = resolution.getHeight();
   m_bitsPerPixel = resolution.getBitsPerPixel();

   m_currentXOffset = 0;
   m_currentYOffset = 0;

   initSDL();
   m_openGLExtensions.initialize();
   initRocket();
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
      DEBUG("Couldn't initialize SDL: %s\n", SDL_GetError());
      exit(1);
   }

   // Initialize SDL_mixer audio library
   if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
   {
      DEBUG("Unable to open audio: %s\n", SDL_GetError());
      exit(1);
   }

   // Enable the OpenGL double buffer
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   // On exit, run the SDL cleanup
   atexit (SDL_Quit);

   std::string* errorMsg = nullptr;
   if(!initSDLVideoMode(errorMsg))
   {
      DEBUG("Couldn't set %dx%dx%d video mode: %s\n", m_width, m_height, m_bitsPerPixel, errorMsg->c_str());
      exit(1);
   }

   if(errorMsg != nullptr)
   {
      delete errorMsg;
      errorMsg = nullptr;
   }
}

bool GraphicsUtil::initSDLVideoMode(std::string*& errorMsg)
{
   // Set video mode based on user's choice of resolution
   unsigned int windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
   if(m_fullScreenEnabled)
   {
      windowFlags |= SDL_WINDOW_FULLSCREEN;
   }

   if(m_window != nullptr)
   {
      SDL_DestroyWindow(m_window);
   }

   m_window = SDL_CreateWindow(
      "EDEn",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      m_width,
      m_height,
      static_cast<SDL_WindowFlags>(windowFlags));

   if(m_window == nullptr)
   {
      errorMsg = new std::string(SDL_GetError());
      return false;
   }

   if(m_openGLContext != nullptr)
   {
      SDL_GL_MakeCurrent(m_window, m_openGLContext);
   }
   else
   {
      m_openGLContext = SDL_GL_CreateContext(m_window);
      if(!m_openGLContext)
      {
         errorMsg = new std::string(SDL_GetError());
         return false;
      }
   }

   // Enable Texture Mapping
   glEnable(GL_TEXTURE_2D);
   
   // Sprites drawn to screen replace whatever is behind them (tiles, background)
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
   
   // Set up the viewport and reset the projection matrix
   glViewport(0, 0, m_width, m_height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   // Set the clear color to black
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
   
   // Create a 2D orthogonal perspective (better for 2D games)
   glOrtho(0.0f, (float)m_width, (float)m_height, 0.0f, -1, 1);

   glMatrixMode(GL_MODELVIEW);

   return true;
}

void GraphicsUtil::initRocket()
{
   Rocket::Core::SetSystemInterface(&m_rocketSystemInterface);
   Rocket::Core::SetRenderInterface(&m_rocketRenderInterface);
   Rocket::Core::RegisterPlugin(&m_rocketContextRegistry);
   Rocket::Core::Initialise();
   Rocket::Controls::Initialise();

   const std::string fontLocation = "data/fonts";
   struct dirent *entry;
   DIR *dp;
   dp = opendir(fontLocation.c_str());
   if (dp == nullptr)
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

Rocket::Core::Context* GraphicsUtil::createRocketContext(const std::string& name)
{
   return Rocket::Core::CreateContext(name.c_str(), Rocket::Core::Vector2i(m_width, m_height));
}

void GraphicsUtil::flipScreen()
{
   SDL_GL_SwapWindow(m_window);
}

bool GraphicsUtil::isVideoModeRefreshRequired() const
{
   const Settings& currentSettings = Settings::getCurrentSettings();
   const Settings::Resolution& currentSettingsResolution = currentSettings.getResolution();
   
   return
      currentSettings.isFullScreenEnabled() != m_fullScreenEnabled ||
      currentSettingsResolution.getWidth() != m_width ||
      currentSettingsResolution.getHeight() != m_height ||
      currentSettingsResolution.getBitsPerPixel() != m_bitsPerPixel;
}

bool GraphicsUtil::refreshVideoMode(std::string*& errorMsg)
{
   const Settings& currentSettings = Settings::getCurrentSettings();
   const Settings::Resolution& currentSettingsResolution = currentSettings.getResolution();

   m_fullScreenEnabled = currentSettings.isFullScreenEnabled();
   m_width = currentSettingsResolution.getWidth();
   m_height = currentSettingsResolution.getHeight();
   m_bitsPerPixel = currentSettingsResolution.getBitsPerPixel();

   bool success = initSDLVideoMode(errorMsg);
   if(success)
   {
      const std::vector<Rocket::Core::Context*>& activeContexts = m_rocketContextRegistry.getActiveContexts();
      for(std::vector<Rocket::Core::Context*>::const_iterator iter = activeContexts.begin(); iter != activeContexts.end(); ++iter)
      {
         (*iter)->SetDimensions(Rocket::Core::Vector2i(m_width, m_height));
      }
   }
   
   return success;
}

const OpenGLExtensions& GraphicsUtil::getExtensions() const
{
   return m_openGLExtensions;
}

int GraphicsUtil::getWidth() const
{
   return m_width;
}

int GraphicsUtil::getHeight() const
{
   return m_height;
}

void GraphicsUtil::clearBuffer()
{
   glMatrixMode(GL_MODELVIEW);
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();
}

void GraphicsUtil::setAbsoluteOffset(int xOffset, int yOffset)
{
   glTranslated(xOffset - m_currentXOffset, yOffset - m_currentYOffset, 0);
   m_currentXOffset = xOffset;
   m_currentYOffset = yOffset;
}

void GraphicsUtil::resetAbsoluteOffset()
{
   glTranslated(-m_currentXOffset, -m_currentYOffset, 0);
   m_currentXOffset = 0;
   m_currentYOffset = 0;
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
         glVertex3f( (float)m_width, 0.0f,          0.0f);
         glVertex3f( (float)m_width, (float)m_height, 0.0f);
         glVertex3f( 0.0f,         (float)m_height, 0.0f);
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
   // Shut down Rocket
   Rocket::Core::Shutdown();

   // Destroy SDL audio and video stuff
   Mix_CloseAudio();

   if(m_openGLContext != nullptr)
   {
      SDL_GL_DeleteContext(m_openGLContext);
   }

   if(m_window != nullptr)
   {
      SDL_DestroyWindow(m_window);
   }

   SDL_Quit();
}
