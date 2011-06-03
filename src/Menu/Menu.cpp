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
#include "Character.h"

#include "ItemsPane.h"
#include "HomePane.h"
#include "EquipPane.h"
#include "CharacterPane.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

Menu::Menu(PlayerData& playerData) : playerData(playerData)
{
   try
   {
      const gcn::Rectangle menuAreaRect(0, 0, top->getWidth() * 0.8, top->getHeight());

      gcn::TabbedArea* menuTabs = new gcn::TabbedArea();

      gcn::Container* menuArea = new gcn::Container();
      menuArea->setOpaque(true);
      menuArea->setDimension(menuAreaRect);

      menuTabs->addTab("Party", menuArea);

      CharacterList party = playerData.getParty();
      for (CharacterList::iterator iter = party.begin(); iter != party.end(); ++iter)
      {
         menuTabs->addTab(iter->first, menuArea);
      }

      HomePane* partyPanel = new HomePane(playerData, menuAreaRect);
      menuArea->add(partyPanel);
      menuPanes[HOME_PANEL] = partyPanel;

      ItemsPane* itemsPanel = new ItemsPane(playerData, menuAreaRect);
      menuArea->add(itemsPanel);
      menuPanes[ITEM_PANEL] = itemsPanel;
      itemsPanel->setVisible(false);

      menuTabs->setDimension(menuAreaRect);
      top->add(menuTabs, top->getWidth() * 0.2, 0);

      populateOpsList();
      actionsListBox = new edwt::ListBox(listOps);
      actionsListBox->adjustSize();
      actionsListBox->setWidth((top->getWidth() * 0.2) - 10);
      actionsListBox->setOpaque(true);

      top->add(actionsListBox, 5, 10);

      showPanel(HOME_PANEL);
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
   listOps->add("Equip", ITEM_PANEL);//EQUIP_PANEL);
   listOps->add("Status", ITEM_PANEL);//STATUS_PANEL);
   listOps->add("Skills", ITEM_PANEL);//SKILLS_PANEL);
   listOps->add("Formation", ITEM_PANEL);//FORMATION_PANEL);
   listOps->add("Party Change", ITEM_PANEL);//PARTY_CHANGE_PANEL);
   listOps->add("Options", ITEM_PANEL);//OPTIONS_PANEL);
   listOps->add("Data", ITEM_PANEL);//DATA_PANEL);
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
      case SDL_USEREVENT:
      {
         if (event.user.data1 == actionsListBox)
         {
            showPanel((MenuPanelType)event.user.code);
         }

         break;
      }
      case SDL_KEYDOWN:
      {
         switch(event.key.keysym.sym)
         {
            case SDLK_ESCAPE:
            {
               popPanel();
               finishState = activePaneStack.empty();
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

void Menu::showPanel(MenuPanelType panelToShow)
{
   if(!activePaneStack.empty())
   {
      if(activePaneStack.top() == panelToShow)
      {
         return;
      }

      menuPanes[activePaneStack.top()]->setVisible(false);
   }

   activePaneStack.push(panelToShow);
   menuPanes[panelToShow]->setVisible(true);
}

void Menu::popPanel()
{
   if(activePaneStack.empty()) return;

   menuPanes[activePaneStack.top()]->setVisible(false);
   activePaneStack.pop();

   if(!activePaneStack.empty())
   {
      menuPanes[activePaneStack.top()]->setVisible(true);
   }
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
