/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2016 Noam Chitayat. All rights reserved.
 */

#include "GraphicsUtil.h"

#include <dirent.h>

#include <SDL.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#include <Rocket/Core.h>
#include <Rocket/Controls.h>

#include "RocketSDLInputMapping.h"
#include "Settings.h"
#include "Size.h"
#include "Sound.h"

#include "DebugUtils.h"

#define DEBUG_FLAG DEBUG_GRAPHICS

void GraphicsUtil::initialize()
{
   m_window = nullptr;
   m_openGLContext = nullptr;
   const auto& resolution = Settings::getCurrentSettings().getResolution();
   m_fullScreenEnabled = Settings::getCurrentSettings().isFullScreenEnabled();
   m_width = resolution.width;
   m_height = resolution.height;
   m_bitsPerPixel = resolution.bitsPerPixel;

   m_currentXOffset = 0;
   m_currentYOffset = 0;

   initSDL();
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
   if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
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

   Mix_ChannelFinished(&Sound::channelFinished);

   // Enable the OpenGL double buffer
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   // On exit, run the SDL cleanup
   atexit (SDL_Quit);

   auto videoModeChangeResult = initSDLVideoMode();
   if(!std::get<0>(videoModeChangeResult))
   {
      DEBUG("Couldn't set %dx%dx%d video mode: %s\n", m_width, m_height, m_bitsPerPixel, std::get<1>(videoModeChangeResult).c_str());
      exit(1);
   }

   m_openGLExtensions.initialize();

   DEBUG("Using GLEW %s", glewGetString(GLEW_VERSION));
}

std::tuple<bool, std::string> GraphicsUtil::initSDLVideoMode()
{
   std::string errorMsg;

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
      errorMsg = std::string(SDL_GetError());
      return { false, errorMsg };
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
         errorMsg = std::string(SDL_GetError());
         return { false, errorMsg };
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

   return { true, errorMsg };
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
   const auto& currentSettings = Settings::getCurrentSettings();
   const auto& currentResolution = currentSettings.getResolution();

   return
      currentSettings.isFullScreenEnabled() != m_fullScreenEnabled ||
      currentResolution.width != m_width ||
      currentResolution.height != m_height ||
      currentResolution.bitsPerPixel != m_bitsPerPixel;
}

std::tuple<bool, std::string> GraphicsUtil::refreshVideoMode()
{
   const auto& currentSettings = Settings::getCurrentSettings();
   const auto& currentResolution = currentSettings.getResolution();

   m_fullScreenEnabled = currentSettings.isFullScreenEnabled();
   m_width = currentResolution.width;
   m_height = currentResolution.height;
   m_bitsPerPixel = currentResolution.bitsPerPixel;

   auto videoModeChangeResult = initSDLVideoMode();
   if(std::get<0>(videoModeChangeResult))
   {
      const std::vector<Rocket::Core::Context*>& activeContexts = m_rocketContextRegistry.getActiveContexts();
      for(auto& context : activeContexts)
      {
         context->SetDimensions(Rocket::Core::Vector2i(m_width, m_height));
      }
   }

   return videoModeChangeResult;
}

OpenGLExtensions& GraphicsUtil::getExtensions()
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
