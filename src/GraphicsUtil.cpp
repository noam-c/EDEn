#include "GraphicsUtil.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "SDL_opengl.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include <guichan.hpp>
#include <guichan/sdl.hpp>
#include <guichan/opengl.hpp>
#include <guichan/opengl/openglsdlimageloader.hpp>
#include "OpenGLTTF.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_GRAPHICS;

SDL_Surface* GraphicsUtil::screen = NULL;

void GraphicsUtil::initialize()
{  initSDL();
   initGuichan();
}

void GraphicsUtil::initSDL()
{  char *msg;

   // Audio information (HARDCODED)
   int audio_rate = 44100;
   Uint16 audio_format = AUDIO_S16SYS; /* 16-bit stereo */
   int audio_channels = 2;
   int audio_buffers = 2048;

   // Initialize SDL audio and video bindings
   if (SDL_Init (SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
   {  printf ("Couldn't initialize SDL: %s\n", SDL_GetError ());
      exit(1);
   }

   // Initialize SDL_mixer audio library
   if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers))
   {  printf("Unable to open audio: %s\n", SDL_GetError());
      exit(1);
   }

   // Enable the OpenGL double buffer
   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

   // On exit, run the SDL cleanup
   atexit (SDL_Quit);
 
   // Resolution (HARDCODED)
   const int width = 800, height = 600;

   // Set 800x600 32-bits video mode (HARDCODED)
   screen = SDL_SetVideoMode (width, height, 32, SDL_HWSURFACE | SDL_OPENGL | SDL_HWACCEL);
   if (screen == NULL)
   {  printf ("Couldn't set 800x600x32 video mode: %s\n", SDL_GetError());
      exit(1);
   }

   // Enable Texture Mapping
   glEnable(GL_TEXTURE_2D);

   // Sprite draws to screen replace whatever is behind them
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
   {  printf("TTF_Init: %s\n", TTF_GetError());
      exit(1);
   }

	// We want unicode
   SDL_EnableUNICODE(1);
   // We want to enable key repeat
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

   SDL_WM_SetCaption ("Buhr Prototype", NULL);
}

void GraphicsUtil::initGuichan()
{  imageLoader = new gcn::OpenGLSDLImageLoader();

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
   // Load the image font.
   font = new edwt::OpenGLTrueTypeFont("data/fonts/LDSRegular.ttf", 16);

	// The global font is static and must be set.
	gcn::Widget::setGlobalFont(font);
}

void GraphicsUtil::flipScreen()
{  glFlush();
   SDL_GL_SwapBuffers();
}

void GraphicsUtil::loadGLTexture(const char* path, GLuint& texture, int& w, int& h)
{  // Create storage space for the texture and load the image
   DEBUG("Loading image %s...", path);
   SDL_Surface *image = IMG_Load(path);

   if(!image)
   {  // Problem loading the image; throw an exception
      throw (EXCEPTION_INFO, std::string("Unable to load image: ") + IMG_GetError());
   }

   DEBUG("Image load successful!");

   Uint32 rmask, gmask, bmask, amask;

/* SDL interprets each pixel as a 32-bit number, 
   so our masks must depend on the endianness 
   (byte order) of the machine */

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			rmask = 0xff000000;
			gmask = 0x00ff0000;
			bmask = 0x0000ff00;
			amask = 0x000000ff;
#else
			rmask = 0x000000ff;
			gmask = 0x0000ff00;
			bmask = 0x00ff0000;
			amask = 0xff000000;
#endif

   SDL_Surface* rgbSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, image->w, 
                                 image->h, 32,
                                 rmask, gmask, bmask, amask);

   if(!rgbSurface)
   {  // Problem converting the image; throw an exception
      throw (EXCEPTION_INFO, std::string("Unable to convert image: ") + SDL_GetError());
   }

   SDL_BlitSurface(image, 0, rgbSurface, 0);

   w = image->w;
   h = image->h;

   // Create the texture
   DEBUG("Generating texture...");
   glGenTextures(1, &texture);

   // Load image into the texture

   // Bind this texture as the current texture OpenGL should work with
   // Any texture ops on GL_TEXTURE_2D will become associated with this texture
   DEBUG("Binding GL texture");
   glBindTexture(GL_TEXTURE_2D, texture);

   // Add Linear filtering for the texture
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   // Transfer the image data into the texture
   DEBUG("Transferring image data to GL texture");
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 
           (void*)rgbSurface->pixels);

   DEBUG("Freeing image surface");
   SDL_FreeSurface(image);

   DEBUG("Freeing RGBA surface");
   SDL_FreeSurface(rgbSurface);

   DEBUG("Texture creation complete.");
}

void GraphicsUtil::setInterface(gcn::Container* top)
{  gui->setTop(top);
}

void GraphicsUtil::stepGUI()
{  gui->logic();
}

void GraphicsUtil::drawGUI()
{  gui->draw();
   // Update the screen
   SDL_GL_SwapBuffers();
}

void GraphicsUtil::pushInput(SDL_Event event)
{  input->pushInput(event);
}

void GraphicsUtil::clearBuffer()
{  glMatrixMode(GL_MODELVIEW);
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();
}

void GraphicsUtil::FadeToColor(float red, float green, float blue, int delay)
{  long time = SDL_GetTicks();
   float alpha = 0.0f;

   GLint oldSrcFactor, oldDstFactor;
   glGetIntegerv(GL_BLEND_SRC, &oldSrcFactor);
   glGetIntegerv(GL_BLEND_DST, &oldDstFactor);
   bool blendEnabled = glIsEnabled(GL_BLEND);
   bool dTestEnabled = glIsEnabled(GL_DEPTH_TEST);
   bool tex2dEnabled = glIsEnabled(GL_TEXTURE_2D);

   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   if(!blendEnabled) glEnable(GL_BLEND);
   if(dTestEnabled)  glDisable(GL_DEPTH_TEST);
   if(tex2dEnabled)  glDisable(GL_TEXTURE_2D);

   for (;;)
   {  glBegin(GL_QUADS);
         glColor4f(red,green,blue,alpha);
         glVertex3f(0.0f,10.0f,1.0f);
         glVertex3f(10.0f,10.0f,1.0f);
         glVertex3f(10.0f,0.0f,1.0f);
         glVertex3f(0.0f,0.0f,1.0f);
      glEnd();

      GraphicsUtil::getInstance()->flipScreen();
 
      long timePassed = SDL_GetTicks() - time;
      alpha = ((timePassed*1.0f)/delay);

      //We're done when alpha reaches 1.0
      if (alpha >= 1.0f)
      {  glColor3f(1.0f,1.0f,1.0f);
         break;
      }
   }

   if(tex2dEnabled)  glEnable(GL_TEXTURE_2D);
   if(dTestEnabled)  glEnable(GL_DEPTH_TEST);
   if(!blendEnabled) glDisable(GL_BLEND);
   glBlendFunc(oldSrcFactor,oldDstFactor);
}

void GraphicsUtil::finish()
{  //Destroys some Guichan stuff
   delete font;
   delete gui;
   delete imageLoader;
   delete graphics;
   delete input;

   //Destroy the SDL_ttf stuff
   if(!TTF_WasInit())
   {  TTF_Quit();
   }

   // Destroy SDL stuff
   SDL_Quit();
}
