#include "MainMenu.h"
#include "GraphicsUtil.h"

#include "TileEngine.h"

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "Container.h"
#include "Label.h"
#include "OpenGLTTF.h"
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

MainMenu::MainMenu()
{
   try
   {
      bgImg = gcn::Image::load("data/images/splash.bmp");
      bg = new gcn::Icon(bgImg);
      top->add(bg,0,0);

      populateOpsList();

      titleLabel = new edwt::Label("Exodus Draconis");
      actionsListBox = new edwt::TitleScreenListBox(titleOps);

      titleFont = new edwt::OpenGLTrueTypeFont("data/fonts/FairyDustB.ttf", 64);
      actionsFont = new edwt::OpenGLTrueTypeFont("data/fonts/FairyDustB.ttf", 32);

      titleLabel->setForegroundColor(gcn::Color(255,255,255));
      titleLabel->setFont(titleFont);
      titleLabel->adjustSize();

      actionsListBox->setFont(actionsFont);

      actionsListBox->adjustSize();
      actionsListBox->adjustWidth();
      actionsListBox->setOpaque(false);

      chooseSound = ResourceLoader::getSound("choose");
      reselectSound = ResourceLoader::getSound("reselect");

      actionsListBox->setReselectSound(reselectSound);

      top->add(titleLabel, 400 - titleLabel->getWidth() / 2, 50);
      top->add(actionsListBox, 400 - actionsListBox->getWidth() / 2, 600 - (actionsListBox->getHeight() + 50));

      #ifndef MUSIC_OFF
      music = ResourceLoader::getMusic("title.mp3");
      #endif
   }
   catch (gcn::Exception e)
   {
      DEBUG(e.getMessage());
      DEBUG(IMG_GetError());
   }
}

void MainMenu::activate()
{
   GameState::activate();

   try
   {
      actionsListBox->requestFocus();
   }
   catch (gcn::Exception e)
   {
      DEBUG(e.getMessage());
      DEBUG(IMG_GetError());
   }
}

void MainMenu::populateOpsList()
{
   titleOps = new edwt::StringListModel();

   titleOps->add("New Game", NEW_GAME_ACTION);
   titleOps->add("Load Game", LOAD_GAME_ACTION);
   titleOps->add("Options", OPTIONS_ACTION);
   titleOps->add("About", ABOUT_ACTION);
   titleOps->add("Quit", QUIT_GAME_ACTION);
}

bool MainMenu::step()
{
   GameState::step();

   if(finished) return false;

   bool done = false;

#ifndef MUSIC_OFF
   music->play();
#endif

   SDL_Event event;

   /* Check for events */
   SDL_WaitEvent(&event);

   switch (event.type)
   {
      case SDL_USEREVENT:
      {
         switch(event.user.code)
         {
            case NEW_GAME_ACTION:
            {
               NewGameAction();
               break;
            }
            case LOAD_GAME_ACTION:
            {
               LoadGameAction();
               break;
            }
            case OPTIONS_ACTION:
            {
               OptionsAction();
               break;
            }
            case ABOUT_ACTION:
            {
               AboutAction();
               break;
            }
            case QUIT_GAME_ACTION:
            {
               QuitAction();
               break;
            }
         }

         break;
      }
      case SDL_KEYDOWN:
      {
         switch(event.key.keysym.sym)
         {
            case SDLK_ESCAPE:
            {
               done = true; break;
            }
         }

         break;
      }
      case SDL_QUIT:
      {
          done = true;
          break;
      }
      default:
      {
          break;
      }
   }

   GraphicsUtil::getInstance()->pushInput(event);

   return !done;
}

void MainMenu::draw()
{
   GameState::draw();

   // Make sure everything is displayed on screen
   GraphicsUtil::getInstance()->flipScreen();

   /* Don't run too fast */
   SDL_Delay (1);
}

MainMenu::~MainMenu()
{
   Music::fadeOutMusic(1000);
   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);

   delete bg;
   delete bgImg;
   delete actionsListBox;
   delete titleLabel;
   delete actionsFont;
   delete titleFont;
   delete titleOps;
}
