#include "Menu.h"
#include "GraphicsUtil.h"

#include "ResourceLoader.h"
#include "Music.h"
#include "Sound.h"

#include "Container.h"
#include "Label.h"
#include "TabbedArea.h"
#include "Tab.h"
#include "OpenGLTTF.h"
#include "StringListModel.h"
#include "ListBox.h"
#include "PlayerData.h"
#include "Character.h"

#include "ItemsMenu.h"
#include "StatusMenu.h"
#include "HomePane.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

Menu::Menu(PlayerData& playerData) : playerData(playerData), characterAction(NONE)
{
   try
   {
      const gcn::Color menuBackgroundColor(50, 50, 50, 150);
      const gcn::Rectangle menuAreaRect(0, 0, top->getWidth() * 0.8 - 5, top->getHeight() - 10);

      bg = new gcn::Icon("data/images/menubg.jpg");

      menuTabs = new edwt::TabbedArea();
      menuArea = new edwt::Container();
      menuArea->setDimension(menuAreaRect);
      menuArea->setOpaque(false);

      menuTabs->setDimension(menuAreaRect);
      menuTabs->setForegroundColor(0xFFFFFF);
      menuTabs->setBaseColor(menuBackgroundColor);
      menuTabs->setDimension(menuAreaRect);

      menuTabs->addTab("Party", menuArea);

      CharacterList party = playerData.getParty();
      for (CharacterList::iterator iter = party.begin(); iter != party.end(); ++iter)
      {
         menuTabs->addTab(iter->first, menuArea);
      }

      homePanel = new HomePane(playerData, menuAreaRect);
      homePanel->setCharacterSelectListener(this);
      menuArea->add(homePanel);

      populateOpsList();
      actionsListBox = new edwt::ListBox(listOps);
      actionsListBox->adjustSize();
      actionsListBox->setWidth((top->getWidth() * 0.2) - 10);
      actionsListBox->setBackgroundColor(menuBackgroundColor);
      actionsListBox->setOpaque(true);
      actionsListBox->setRowPadding(5);

      top->add(bg);
      top->add(menuTabs, top->getWidth() * 0.2, 5);
      top->add(actionsListBox, 5, menuTabs->getTabHeight() + 5);
   }
   catch (gcn::Exception e)
   {
      DEBUG(e.getMessage());
   }
}

void Menu::populateOpsList()
{
   listOps = new edwt::StringListModel();

   listOps->add("Items", ITEM_PANEL);
   listOps->add("Equip", EQUIP_PANEL);
   listOps->add("Status", STATUS_PANEL);
   listOps->add("Skills", SKILLS_PANEL);
   listOps->add("Formation", FORMATION_PANEL);
   listOps->add("Party Change", PARTY_CHANGE_PANEL);
   listOps->add("Options", OPTIONS_PANEL);
   listOps->add("Data", DATA_PANEL);
}

void Menu::activate()
{
   GameState::activate();
   homePanel->setVisible(true);
}

bool Menu::step()
{
   if(finished) return false;

   bool done = false;

   pollInputEvent(done);

   return !done;
}

void Menu::pollInputEvent(bool& finishState)
{
   SDL_Event event;

   /* Check for events */
   if(SDL_PollEvent(&event))
   {
      switch (event.type)
      {
         case SDL_USEREVENT:
         {
            if (event.user.data1 == actionsListBox)
            {
               showPanel((MenuAction)event.user.code);
            }

            break;
         }
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
}

bool Menu::isCharacterDependent(MenuAction action)
{
   switch(action)
   {
      case EQUIP_PANEL:
	   case STATUS_PANEL:
	   case SKILLS_PANEL:
         return true;

	   case FORMATION_PANEL:
	   case PARTY_CHANGE_PANEL:
	   case OPTIONS_PANEL:
	   case DATA_PANEL:
      case ITEM_PANEL:
	   default:
         return false;
   }
}

void Menu::showPanel(MenuAction panelToShow)
{
   if(isCharacterDependent(panelToShow))
   {
      characterAction = panelToShow;
   }
   else
   {
      ExecutionStack::getInstance()->pushState(new ItemsMenu(top, *menuArea, playerData));
      homePanel->setVisible(false);
   }
}

void Menu::characterSelected(const std::string& characterName)
{
   switch(characterAction)
   {
      case STATUS_PANEL:
         Character* selected = playerData.getParty()[characterName];
         ExecutionStack::getInstance()->pushState(new StatusMenu(top, *menuArea, *selected));
         homePanel->setVisible(false);
         break;
   }
}

void Menu::draw()
{
   /* Don't run too fast */
   SDL_Delay (1);
}

Menu::~Menu()
{
   delete homePanel;
   delete menuArea;
   delete menuTabs;
	delete listOps;
	delete actionsListBox;
   delete bg;
}
