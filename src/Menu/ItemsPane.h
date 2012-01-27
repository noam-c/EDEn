/*
 *  This file is covered by the Ruby license. See LICENSE.txt for more details.
 *
 *  Copyright (C) 2007-2012 Noam Chitayat. All rights reserved.
 */

#ifndef ITEMS_PANE_H
#define ITEMS_PANE_H

#include "MenuPane.h"
#include "ListBox.h"

class ItemListModel;
class PlayerData;

namespace edwt
{
   class ModuleSelectListener;
};

/**
 * The pane to display when showing the items menu state.
 *
 * @author Noam Chitayat
 */
class ItemsPane : public MenuPane, public gcn::ActionListener
{
   bool invalidated;

   /** The model holding the item list. */
   ItemListModel& itemListModel;

   /** A listbox containing all the items in the inventory. */
   edwt::ListBox listBox;
   
   /** The module selection listener to call when an item is selected. */
   edwt::ModuleSelectListener* moduleSelectListener;
   
   /**
    * Refresh of the list of items from the player data.
    */
   void refresh();

   protected:
      void logic();
   
   public:
      /** The event ID for an item list selection event. */
      static const std::string ItemListEventId;

      /**
       * Constructor.
       *
       * @param itemList The items display in the pane.
       * @param rect The preferred dimensions of this pane.
       */
      ItemsPane(ItemListModel& itemList, const gcn::Rectangle& rect);
      
      void setModuleSelectListener(edwt::ModuleSelectListener* listener);

      /**
       * Response to UI actions.
       *
       * @param event The GUI action event.
       */
      void action(const gcn::ActionEvent& event);

      /**
       * Request a refresh of the list of items from the player data.
       */
      void invalidate();

      /**
       * Destructor.
       */
      ~ItemsPane();
};

#endif
