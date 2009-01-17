#include "GraphicsUtil.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL.h>
#include "SDL_opengl.h"
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
{   initSDL();
    initGuichan();
}

void GraphicsUtil::initSDL()
{   char *msg;

    // Audio information (HARDCODED)
    int audio_rate = 44100;
    Uint16 audio_format = AUDIO_S16SYS; /* 16-bit stereo */
    int audio_channels = 2;
    int audio_buffers = 2048;

    // Initialize SDL audio and video bindings
    if (SDL_Init (SDL_INIT_AUDIO|SDL_INIT_VIDEO) < 0)
    {   printf ("Couldn't initialize SDL: %s\n", SDL_GetError ());
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
    {   printf ("Couldn't set 800x600x32 video mode: %s\n", SDL_GetError());
        exit(1);
    }

    // Enable Texture Mapping
    glEnable(GL_TEXTURE_2D);

    // Set up the viewport and reset the projection matrix
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Set the clear color to black
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Create a 2D orthogonal perspective (better for 2D games)
    gluOrtho2D(0.0f, 10.0f, 0.0f, 10.0f);    
    //gluPerspective( 45.0, width/height, 1.0, 20.0 );

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
{   imageLoader = new gcn::OpenGLSDLImageLoader();

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
{   glFlush();
    SDL_GL_SwapBuffers();
}

void GraphicsUtil::DrawIMG(SDL_Surface *img, int x, int y)
{   SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    SDL_BlitSurface(img, NULL, screen, &dest);
}

SDL_Surface* GraphicsUtil::ImageLoad(char *file)
{   SDL_Surface *temp1, *temp2;
    temp1 = SDL_LoadBMP(file);
    temp2 = SDL_DisplayFormat(temp1);
    SDL_FreeSurface(temp1);

    return temp2;
}

void GraphicsUtil::setInterface(gcn::Container* top)
{   gui->setTop(top);
}

void GraphicsUtil::stepGUI()
{   gui->logic();
}

void GraphicsUtil::drawGUI()
{   gui->draw();
    // Update the screen
    SDL_GL_SwapBuffers();
}

void GraphicsUtil::pushInput(SDL_Event event)
{   input->pushInput(event);
}

void GraphicsUtil::clearBuffer()
{  glMatrixMode(GL_MODELVIEW);
   glClear(GL_COLOR_BUFFER_BIT);
   glLoadIdentity();
}

void GraphicsUtil::FadeToColor(float red, float green, float blue, int delay)
{   long time = SDL_GetTicks();
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
{   //Destroys some Guichan stuff
    delete font;
    delete gui;
    delete imageLoader;
    delete graphics;
    delete input;

    //Destroy the SDL_ttf stuff
    if(!TTF_WasInit()) {
        TTF_Quit();
    }

    // Destroy SDL stuff
 	 SDL_Quit();
}
