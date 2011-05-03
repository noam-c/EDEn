#include "Menu.h"
#include "GraphicsUtil.h"

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "Container.h"
#include "Label.h"
#include "OpenGLTTF.h"
#include "StringListModel.h"
#include "TitleScreenListBox.h"
#include "PlayerData.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

Menu::Menu(PlayerData* playerData)
{
   try
   {
      gcn::TabbedArea* menuPanel = new gcn::TabbedArea();
      gcn::Container* character1 = new gcn::Container();
      gcn::Container* character2 = new gcn::Container();
      gcn::Container* character3 = new gcn::Container();

      menuPanel->addTab("Kain", character1);
      menuPanel->addTab("Shaka", character2);
      menuPanel->addTab("Akrom", character3);

	  menuPanel->setOpaque(true);
	  menuPanel->setDimension(top->getDimension());

      top->add(menuPanel, 0, 0);
   }
   catch (gcn::Exception e)
   {
      DEBUG(e.getMessage());
   }
}

void Menu::activate()
{
   GameState::activate();
}

bool Menu::step()
{
   if(finished) return false;

   bool done = false;

   waitForInputEvent(done);

   return !done;
}

void Menu::waitForInputEvent(bool& finishState)
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
         }

         break;
      }
      default:
      {
          break;
      }
   }

   // If the main menu didn't consume this event, then propagate to the generic input handling
   handleEvent(event);
}

void Menu::draw()
{
   /* Don't run too fast */
   SDL_Delay (1);
}

Menu::~Menu()
{
}
