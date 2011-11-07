#ifndef ITEMS_PANE_H_
#define ITEMS_PANE_H_

#include "MenuPane.h"

class ItemListModel;
class PlayerData;

namespace edwt
{
   class ListBox;
};

/**
 * The pane to display when showing the items menu state.
 *
 * @author Noam Chitayat
 */
class ItemsPane : public MenuPane
{
   /** The player data with the inventory to display. */
   PlayerData& playerData;

   /** A listbox containing all the items in the inventory. */
   edwt::ListBox* listBox;

   /** The model holding the item list. */
   ItemListModel* itemsList;

   public:
      /**
       * Constructor.
       *
       * @param playerData The player data to display in the pane.
       * @param rect The preferred dimensions of this pane.
       */
      ItemsPane(PlayerData& playerData, const gcn::Rectangle& rect);

      /**
       * Refresh the list of items from the player data.
       */
      void refresh();

      /**
       * Destructor.
       */
      ~ItemsPane();
};

#endif