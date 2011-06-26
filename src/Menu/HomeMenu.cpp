#include "HomeMenu.h"
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

#include "MenuShell.h"
#include "ItemsMenu.h"
#include "StatusMenu.h"
#include "HomePane.h"

#include "ExecutionStack.h"
#include "SDL_image.h"
#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

HomeMenu::HomeMenu(MenuShell& menuShell, PlayerData& playerData) : MenuState(menuShell), playerData(playerData), characterAction(NONE)
{
   setMenuPane(new HomePane(playerData, menuShell.getDimension()));
}

bool HomeMenu::step()
{
   if(finished) return false;

   bool done = false;

   pollInputEvent(done);

   return !done;
}

void HomeMenu::pollInputEvent(bool& finishState)
{
   SDL_Event event;

   /* Check for events */
   if(SDL_PollEvent(&event))
   {
      switch (event.type)
      {
         case SDL_USEREVENT:
         {
            if (event.user.data1 == menuShell.actionsListBox)
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

      // If the main HomeMenu didn't consume this event, then propagate to the generic input handling
      handleEvent(event);
   }
}

bool HomeMenu::isCharacterDependent(MenuAction action)
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

void HomeMenu::showPanel(MenuAction panelToShow)
{
   if(isCharacterDependent(panelToShow))
   {
      characterAction = panelToShow;
   }
   else
   {
      ExecutionStack::getInstance()->pushState(new ItemsMenu(menuShell, playerData));
      menuPane->setVisible(false);
   }
}

void HomeMenu::characterSelected(const std::string& characterName)
{
   switch(characterAction)
   {
      case STATUS_PANEL:
         ExecutionStack::getInstance()->pushState(new StatusMenu(menuShell, playerData, characterName));
         menuPane->setVisible(false);
         break;
   }
}

void HomeMenu::tabChanged(const std::string& tabName)
{
   ExecutionStack::getInstance()->pushState(new StatusMenu(menuShell, playerData, tabName));
   menuPane->setVisible(false);
}

HomeMenu::~HomeMenu()
{
}
