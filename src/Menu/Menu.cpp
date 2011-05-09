#include "Menu.h"
#include "GraphicsUtil.h"

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "Container.h"
#include "Label.h"
#include "OpenGLTTF.h"
#include "StringListModel.h"
#include "ListBox.h"
#include "PlayerData.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

enum ListActions
{
	ITEM_ACTION,
	EQUIP_ACTION,
	STATUS_ACTION,
	SKILLS_ACTION,
	FORMATION_ACTION,
	PARTY_CHANGE_ACTION,
	OPTIONS_ACTION,
	DATA_ACTION,
};

Menu::Menu(const PlayerData& playerData) : playerData(playerData)
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
	  menuPanel->setHeight(top->getHeight());
	  menuPanel->setWidth(top->getWidth() * 0.8);

      top->add(menuPanel, top->getWidth() * 0.2, 0);

      populateOpsList();
	  
      actionsListBox = new edwt::ListBox(listOps);
      actionsListBox->adjustSize();
      actionsListBox->adjustWidth();
	  actionsListBox->setOpaque(true);

	  top->add(actionsListBox, 0, 0);
   }
   catch (gcn::Exception e)
   {
      DEBUG(e.getMessage());
   }
}

void Menu::populateOpsList()
{
   listOps = new edwt::StringListModel();

   listOps->add("Items", ITEM_ACTION);
   listOps->add("Equip", EQUIP_ACTION);
   listOps->add("Status", STATUS_ACTION);
   listOps->add("Skills", SKILLS_ACTION);
   listOps->add("Formation", FORMATION_ACTION);
   listOps->add("Party Change", PARTY_CHANGE_ACTION);
   listOps->add("Options", OPTIONS_ACTION);
   listOps->add("Data", DATA_ACTION);
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
	delete listOps;
	delete actionsListBox;
}
