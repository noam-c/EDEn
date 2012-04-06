/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "MenuShell.h"
#include "TabbedArea.h"
#include "guichan.hpp"
#include "GraphicsUtil.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "StringListModel.h"
#include "MenuPane.h"
#include "MenuState.h"
#include "TabChangeListener.h"
#include "Sound.h"
#include "ResourceLoader.h"
#include "Character.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

MenuShell::MenuShell(PlayerData& playerData) : activeState(NULL)
{
   try
   {
      setWidth(GraphicsUtil::getInstance()->getWidth());
      setHeight(GraphicsUtil::getInstance()->getHeight());

      const gcn::Color menuBackgroundColor(50, 50, 50, 150);
      const gcn::Rectangle menuAreaRect(0, 0, getWidth() * 0.8 - 5, getHeight() - 10);

      bg = new gcn::Icon("data/images/menubg.jpg");
      selectSound = ResourceLoader::getSound("reselect");

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
         menuTabs->addTab((*iter)->getName(), menuArea);
      }

      populateOpsList();
      actionsListBox = new edwt::ListBox(listOps);
      actionsListBox->setBaseColor(0xFFFFFF);
      actionsListBox->setMinWidth((getWidth() * 0.2) - 10);
      actionsListBox->setAlignment(edwt::RIGHT);
      actionsListBox->adjustSize();
      actionsListBox->adjustWidth();
      actionsListBox->setBackgroundColor(menuBackgroundColor);
      actionsListBox->setOpaque(false);
      actionsListBox->setRowPadding(5);
      actionsListBox->addActionListener(this);
      actionsListBox->addSelectionListener(this);

      add(bg);
      add(menuTabs, getWidth() * 0.2, 5);
      add(actionsListBox, 5, menuTabs->getTabHeight() + 5);
   }
   catch (gcn::Exception& e)
   {
      DEBUG(e.getMessage());
   }
}

void MenuShell::populateOpsList()
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

void MenuShell::addPane(MenuPane* menuPane)
{
   menuArea->add(menuPane);
}

void MenuShell::removePane(MenuPane* menuPane)
{
   menuArea->remove(menuPane);
}

void MenuShell::setActiveState(MenuState* menuState)
{
   menuTabs->setTabChangeListener(menuState);
   activeState = menuState;
}

void MenuShell::action(const gcn::ActionEvent& event)
{
   if(event.getSource() == actionsListBox)
   {
      selectSound->play();

      MenuAction selectedAction = (MenuAction)listOps->getActionAt(actionsListBox->getSelected());
      activeState->showPanel(selectedAction);
   }
}

void MenuShell::valueChanged(const gcn::SelectionEvent& event)
{
   selectSound->play();
}

MenuShell::~MenuShell()
{
   delete menuArea;
   delete menuTabs;
   delete listOps;
   delete actionsListBox;
   delete bg;
}
