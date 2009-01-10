#include "MainMenu.h"
#include "GraphicsUtil.h"
#include "OpenGLTTF.h"
#include "TileEngine.h"
#include "StringListModel.h"
#include "TitleScreenListBox.h"
#include "ExecutionStack.h"
#include "SDL_Image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_TITLE;

#define NEW_GAME_ACTION 0
#define LOAD_GAME_ACTION 1
#define OPTIONS_ACTION 2
#define ABOUT_ACTION 3
#define QUIT_GAME_ACTION 4

/** \todo Get rid of the MUSIC_OFF macro usage once an options menu with
 * persistence is successfully implemented.
 */
// Uncomment this macro to turn off music in the main menu
//#define MUSIC_OFF

MainMenu::MainMenu()
{  top = new gcn::Container();
   top->setDimension(gcn::Rectangle(0, 0, 800, 600));

   try
   {  bgImg = gcn::Image::load("data/images/splash.bmp");
      bg = new gcn::Icon(bgImg);
      top->add(bg,0,0);

      populateOpsList();

      listbox = new edwt::TitleScreenListBox(titleOps);

      titleFont = new edwt::OpenGLTrueTypeFont("data/fonts/FairyDustB.ttf", 32);

      listbox->setFont(titleFont);

      listbox->adjustSize();
      listbox->adjustWidth();
      listbox->setOpaque(false);

      reselectSound = Mix_LoadWAV("data/sounds/reselect.wav");
      chooseSound = Mix_LoadWAV("data/sounds/choose.wav");

      listbox->setReselectSound(reselectSound);

      top->add(listbox, 400 - listbox->getWidth() / 2, 600 - (listbox->getHeight() + 50));

      #ifndef MUSIC_OFF
      music = Mix_LoadMUS("data/music/title.mp3");
      #endif
   }
   catch (gcn::Exception e)
   {  DEBUG(e.getMessage());
      DEBUG(IMG_GetError());
   }
}

void MainMenu::activate()
{  GameState::activate();

   try
   {  listbox->requestFocus();
   }
   catch (gcn::Exception e)
   {  DEBUG(e.getMessage());
      DEBUG(IMG_GetError());
   }
}

void MainMenu::populateOpsList()
{  titleOps = new edwt::StringListModel();

   titleOps->add("New Game", NEW_GAME_ACTION);
   titleOps->add("Load Game", LOAD_GAME_ACTION);
   titleOps->add("Options", OPTIONS_ACTION);
   titleOps->add("About", ABOUT_ACTION);
   titleOps->add("Quit", QUIT_GAME_ACTION);
}

bool MainMenu::step()
{  GameState::step();

   if(finished) return false;

   bool done = false;

   #ifndef MUSIC_OFF
   if(!Mix_PlayingMusic())
   {  Mix_PlayMusic(music, 0);
   }
   #endif

   SDL_Event event;

   /* Check for events */
   SDL_WaitEvent(&event);

   switch (event.type)
   {   case SDL_USEREVENT:
       {  switch(event.user.code)
          {  case NEW_GAME_ACTION:
             {  NewGameAction();
                break;
             }
             case LOAD_GAME_ACTION:
             {  LoadGameAction();
                break;
             }
             case OPTIONS_ACTION:
             {  OptionsAction();
                break;
             }
             case ABOUT_ACTION:
             {  AboutAction();
                break;
             }
             case QUIT_GAME_ACTION:
             {  QuitAction();
                break;
             }
          }

          break;
       }
       case SDL_KEYDOWN:
       {  switch(event.key.keysym.sym)
             case SDLK_ESCAPE:
             {  done = true; break;  }
          break;
       }
       case SDL_QUIT:
       {   done = true;
           break;
       }
       default:
       {   break;
       }
   }

   GraphicsUtil::getInstance()->pushInput(event);

   return !done;
}

void MainMenu::draw()
{  GameState::draw();

   // Make sure everything is displayed on screen
   GraphicsUtil::getInstance()->flipScreen();

   /* Don't run too fast */
   SDL_Delay (1);
}

MainMenu::~MainMenu()
{  Mix_FadeOutMusic(1000);
   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);
    
   delete bg;
   delete bgImg;
   delete listbox;
   delete titleFont;
   delete titleOps;

   Mix_FreeChunk(reselectSound);
   Mix_FreeChunk(chooseSound);

   #ifndef MUSIC_OFF
   Mix_FreeMusic(music);
   #endif
}
