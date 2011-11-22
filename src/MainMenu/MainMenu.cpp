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
#include "ListBox.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_TITLE;

enum MainMenuActions
{
	NEW_GAME_ACTION,
	LOAD_GAME_ACTION,
	OPTIONS_ACTION,
	ABOUT_ACTION,
	QUIT_GAME_ACTION,
	MENU_PROTOTYPE_ACTION,
};

MainMenu::MainMenu(ExecutionStack& executionStack) : GameState(executionStack)
{
   try
   {
      bg = new gcn::Icon("data/images/splash.jpg");
      bg->setDimension(top->getDimension());
      top->add(bg,0,0);

      populateOpsList();

      titleLabel = new edwt::Label("Exodus Draconis Engine");
      actionsListBox = new edwt::ListBox(titleOps);

      titleFont = new edwt::OpenGLTrueTypeFont("data/fonts/FairyDustB.ttf", 64);
      actionsFont = new edwt::OpenGLTrueTypeFont("data/fonts/FairyDustB.ttf", 32);

      titleLabel->setForegroundColor(0x666655);
      titleLabel->setFont(titleFont);
      titleLabel->adjustSize();
      
      actionsListBox->setBaseColor(0xBBBBAA);
      actionsListBox->setHighlightColor(0x333322);

      actionsListBox->addActionListener(this);
      actionsListBox->addSelectionListener(this);
      actionsListBox->setFont(actionsFont);

      actionsListBox->adjustSize();
      actionsListBox->adjustWidth();
      actionsListBox->setAlignment(edwt::CENTER);
      actionsListBox->setOpaque(false);

      chooseSound = ResourceLoader::getSound("choose");
      reselectSound = ResourceLoader::getSound("reselect");

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
   titleOps->add("Menu Prototype", MENU_PROTOTYPE_ACTION);
   titleOps->add("Options", OPTIONS_ACTION);
   titleOps->add("About", ABOUT_ACTION);
   titleOps->add("Quit", QUIT_GAME_ACTION);
}

bool MainMenu::step()
{
   if(finished) return false;

   bool done = false;

#ifndef MUSIC_OFF
   music->play();
#endif

   waitForInputEvent(done);

   return !done;
}

void MainMenu::valueChanged(const gcn::SelectionEvent& event)
{
   reselectSound->play();
}

void MainMenu::action(const gcn::ActionEvent& event)
{
   chooseSound->play();
   if(event.getSource() == actionsListBox)
   {
      switch(titleOps->getActionAt(actionsListBox->getSelected()))
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
         case MENU_PROTOTYPE_ACTION:
         {
            MenuPrototypeAction();
            break;
         }
      }
   }
}

void MainMenu::waitForInputEvent(bool& finishState)
{
   SDL_Event event;

   /* Check for events */
   SDL_WaitEvent(&event);

   switch (event.type)
   {
      case SDL_KEYDOWN:
      {
         switch(event.key.keysym.sym)
         {
            case SDLK_ESCAPE:
            {
               finishState = true;
               return;
            }
            default:
            {
               break;
            }
         }

         break;
      }
      case SDL_QUIT:
      {
          finishState = true;
          return;
      }
      default:
      {
          break;
      }
   }

   // If the main menu didn't consume this event, then propagate to the generic input handling
   handleEvent(event);
}

void MainMenu::draw()
{
   /* Don't run too fast */
   SDL_Delay (1);
}

MainMenu::~MainMenu()
{
   Music::fadeOutMusic(1000);
   GraphicsUtil::getInstance()->FadeToColor(0.0f, 0.0f, 0.0f, 1000);

   delete bg;
   delete actionsListBox;
   delete titleLabel;
   delete actionsFont;
   delete titleFont;
   delete titleOps;
}
