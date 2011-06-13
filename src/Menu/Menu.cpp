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

      HomePane* homePanel = new HomePane(playerData, menuAreaRect);
      menuArea->add(homePanel);
      menuPanes[HOME_PANEL] = homePanel;

      ItemsPane* itemsPanel = new ItemsPane(playerData, menuAreaRect);
      menuArea->add(itemsPanel);
      menuPanes[ITEM_PANEL] = itemsPanel;
      itemsPanel->setVisible(false);

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
   // Delete all the menu panes
   for (std::map<MenuPanelType, MenuPane*>::iterator iter = menuPanes.begin(); iter != menuPanes.end(); ++iter)
   {
      delete iter->second;
   }

   delete menuArea;
   delete menuTabs;
	delete listOps;
	delete actionsListBox;
   delete bg;
}
