/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#include "ItemsPane.h"
#include "ItemData.h"
#include "Item.h"
#include "PlayerData.h"
#include "ListBox.h"
#include "ItemListModel.h"
#include "ModuleSelectListener.h"

#include "DebugUtils.h"

const int debugFlag = DEBUG_MENU;

const std::string ItemsPane::ItemListEventId = "ItemListEvent";

ItemsPane::ItemsPane(ItemListModel& itemList, const gcn::Rectangle& rect) : MenuPane(rect), itemListModel(itemList), listBox(&itemList)
{
   listBox.setNumColumns(2);
   listBox.setMinColumnWidth(0, 200);
   listBox.setMinColumnWidth(1, 20);
   listBox.setColumnAlignment(0, edwt::LEFT);
   listBox.setColumnAlignment(1, edwt::RIGHT);
   listBox.setActionEventId(ItemListEventId);
   listBox.addActionListener(this);

   add(&listBox);
   
   refresh();
}

void ItemsPane::setModuleSelectListener(edwt::ModuleSelectListener* listener)
{
   moduleSelectListener = listener;
}

void ItemsPane::action(const gcn::ActionEvent& event)
{
   if(event.getId() == ItemListEventId)
   {
      moduleSelectListener->moduleSelected(listBox.getSelected(), ItemListEventId);
   }
}

void ItemsPane::logic()
{
   MenuPane::logic();
   if(invalidated)
   {
      refresh();
   }
}

void ItemsPane::invalidate()
{
   invalidated = true;
}

void ItemsPane::refresh()
{
   listBox.adjustSize();
   listBox.adjustWidth();
}

ItemsPane::~ItemsPane()
{
}
